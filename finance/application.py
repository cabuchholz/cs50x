import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
import re


from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    # Show portfolio of stocks

    # Extract the users current holdings
    holdings = db.execute("SELECT symbol, SUM(quantity) AS 'shares' FROM transactions WHERE user_id = ? GROUP BY symbol",
    session['user_id'])

    # For each holding get the current price, and value; And add to total value
    stock_value = 0
    for holding in holdings:
        #Look up the price
        spot = lookup(holding['symbol'])
        price = spot['price']

        #Get total holding value
        value = price*holding['shares']

        #Append back into dictionary
        holding['price'] = usd(price)
        holding['value'] = usd(value)

        #Add to the total stock value
        stock_value = stock_value + value

    #Get the users current cash position
    cash = db.execute("SELECT cash FROM users WHERE ?", session["user_id"])[0]['cash']

    #Sum the users current holding value in total
    total = cash + stock_value

    #Convert Total and cash to USD
    total = usd(total)
    cash = usd(cash)


    return render_template("index.html", holdings=holdings, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    # User reached route via POST
    if request.method == "POST":
        # Get the input from the user
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Check to ensure symbol is null or does not exist
        spot = lookup(symbol)
        if spot is None:
            return apology("Invalid stock symbol", "Please try again")

        # Check to see if Null number of shares was given, or negative
        elif not shares:
            return apology("Need positive number of shares", "Please try again")

        elif int(shares) <= 0:
            return apology("Need positive number of shares", "Please try again")

        # Check to see if the user has enough cash in the account

        else:
            # Get the users total amount of cash and amount being requested
            row = db.execute("SELECT cash FROM users WHERE ?", session["user_id"])
            cash = row[0]['cash']
            total = (spot['price']*int(shares))

            #Check to ensure has enough cash, if so, complete purchase
            if cash >= total:
                #Compute the values to insert
                remain = cash - total
                now = datetime.now()

                #Add the transaction to the table
                db.execute("INSERT INTO transactions (user_id, symbol, quantity, price, time) VALUES (?, ?, ?, ?, ?)",
                session['user_id'], symbol, shares, spot['price'], now)

                #Amend the user's cash position
                db.execute("UPDATE users SET cash = ? WHERE id = ?", remain, session['user_id'])

                #After completed route them to the Index page
                return redirect("/history")

            else:
                return apology("Not enough funds", "Add funds or purchase less")

    # User reached route via GET
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Query all of the users transactions
    transactions = db.execute("SELECT symbol, quantity, price, time FROM transactions WHERE user_id = ?",
    session['user_id'])

    # Add whether it was a purchase or sale
    for transaction in transactions:
        if int(transaction['quantity']) < 0:
            transaction['type'] = "Sale"

        else:
            transaction['type'] = "Purchase"

    # Render the template with the transactions
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    #Handle the Post Route: Show the quoted price for the stock
    if request.method == "POST":

        #Lookup the stock symbol submitted
        symbol = request.form.get("symbol")
        spot = lookup(symbol)

        #Check to see if response is empty, if so render an apology
        if spot is None:
            return apology("Invalid stock symbol", "Please try again")

        #If response is not empty
        else:



            #Get the name and USD price
            name = spot['name']
            price = usd(spot['price'])

            # return back the price
            return render_template("quoted.html", name=name, price=price)

    #Handle the Get Route: Show a form to request a price
    else:
        return render_template("quote.html")


    """Get stock quote."""
    return apology("TODO")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Handle the Post Route
    if request.method == "POST":
        #Get the username from the form
        username = request.form.get("username")

        #Check if username is blank
        if not username:
            return apology("Blank username", "Please enter valid username")

        # Ensure that the user name is at least 3 characters long
        if len(username) < 3:
            return apology("Username too short", "Must be at least 3 characters long")

        #Validate username: Check if blank in DB
        users = db.execute("SELECT username FROM users WHERE username = ?;", username)
        if len(users) != 0:
            return apology("Username already exists", "Please try another username")

        # Get the password and confirmation; Also determine values
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        digits = re.findall("\d", password)
        letters = re.findall("[a-zA-Z]", password)
        special = re.findall("[+*.|()${}%@#]", password)

        #Validate the password: Check if blank
        if not password:
            return apology("Blank password", "Please enter a valid password")

        # Ensure that the password has at least 1 letter, 1 number and 1 symbol
        if len(password) < 8 or len(digits) == 0 or len(letters) == 0 or len(special) == 0:
            return apology("Invalid password", "At least 8 characters, with 1 letter, 1 number, 1 special character, and no whitepaces")

        #Validate the password: Check if  not match the confirmation
        if password != confirmation:
            return apology("Passwords do not match", "Please re-enter your password")

        #Insert valid new user and password into the DB, with hashed password
        phash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, phash)

        #Once sucessfully added, redirect to the login page
        return redirect("/login")

    # Handle the Get Route
    else:
        users = db.execute("SELECT username FROM users;")
        users = len(users)
        return render_template("register.html", users=users)

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    # Sell shares of a stock
    # User reached route via POST
    if request.method == "POST":
        # Get the input from the user
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Check to ensure user picked a stock
        if symbol is None:
            return apology("No Stock Selected", "Please try again")

        # Check to see if Null number of shares was given, or negative
        elif not shares:
            return apology("Need to enter # of shares", "Please try again")

        elif int(shares) <= 0:
            return apology("Need to enter positive # of shares", "Please try again")

        # Check to see if the user owns enough of share

        else:
            # Get the users total holdings of this stock
            holdings = db.execute("SELECT symbol, SUM(quantity) AS 'shares' FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol",
            session['user_id'], symbol)
            holding = holdings[0]['shares']

            #Confirm that number of shares is less than own
            if int(shares) <=  int(holding):

                # Get the users current cash balance
                cash = db.execute("SELECT cash FROM users WHERE ?", session["user_id"])[0]['cash']

                # Compute the value of the sale, and add to cash balance
                spot = lookup(holdings[0]['symbol'])
                price = spot['price']
                total_sale = float(price) * float(shares)
                total_value = cash + total_sale

                #get the current time
                now = datetime.now()

                #Inverse the shares as is a sale
                shares_sale = (0-int(shares))

                #Update transaction table with sale
                db.execute("INSERT INTO transactions (user_id, symbol, quantity, price, time) VALUES (?, ?, ?, ?, ?)",
                session['user_id'], symbol, shares_sale, price, now)

                # Update cash holdings in user table
                db.execute("UPDATE users SET cash = ? WHERE id = ?", total_value, session['user_id'])

                # Redirect to index page
                return redirect("/history")

            else:
                return apology("Not enough shares", "need to sell less")

    # User reached route via GET
    else:
        #Check to see which stocks the user owns
        holdings = db.execute("SELECT symbol, SUM(quantity) AS 'shares' FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(quantity) > 0",
        session['user_id'])

        # Add all of their holdings to a list
        symbols = []
        for holding in holdings:
            symbols.append(holding['symbol'])


        return render_template("sell.html", symbols=symbols)



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
