

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

        else:
            # Check to ensure that the user has enough cash in account
            row = db.execute("SELECT cash FROM users WHERE ?", session["user_id"])
            cash = row[0]['cash']
            request = (spot['price']*int(shares))
            return render_template("buytest.html", request=request)

            #if cash >= request:
             #   remain = cash - request


                # Once purchase is made, redirect to homepage
               # return render_template("buytest.html", remain=remain)

            #else:
             #   return apology("Not enough funds", "Add funds or purchase less")

    # User reached route via GET
    else:
        return render_template("buy.html")


