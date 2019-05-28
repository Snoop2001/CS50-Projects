import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stocks_value = 0
    # save company symbol and total shares for each company
    portfolio = db.execute(
        "SELECT symbol, SUM(shares) FROM stocks WHERE id=:id GROUP BY symbol ORDER BY SUM(shares) DESC", id=session["user_id"])
    
    # sum total value of all stocks
    for row in range(len(portfolio)):
        company_value = round(lookup(portfolio[row]["symbol"])["price"] * portfolio[row]["SUM(shares)"], 2)
        stocks_value += company_value
    
    user_cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])[0]["cash"]
    grand_value = stocks_value + user_cash 
    
    # pass portfolio data into html with necessary functions
    return render_template("index.html", grand_value=grand_value, user_cash=user_cash, portfolio=portfolio, lookup=lookup, range=range, len=len, usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        try:
            result = int(request.form.get('shares'))
        except:
            return apology("Please enter a positive number of shares", 400)
        
        # ensure fields are filled in
        if not request.form.get("symbol"):
            return apology("Please enter a company symbol", 400)
            
        elif int(request.form.get('shares')) < 0:
            return apology("Please enter a positive number of shares", 400)

        # if filled out correctly, begin
        else:
            # save quote data and ensure company exists
            quotes = lookup(request.form.get("symbol"))
            if not quotes:
                return apology("Company not found", 400)
                
            # calculate total cost of purchase
            total = round(quotes["price"] * int(request.form.get("shares")), 2)
            
            # ensure user has enough
            result = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
            user_cash = result[0]['cash']
            if user_cash > total:
                # insert purchase into new table
                db.execute("INSERT INTO stocks (id, symbol, type, price_per_share, shares) VALUES(:id, :symbol, 'bought', :cost, :shares)",
                           id=session["user_id"], symbol=request.form.get("symbol"), cost=quotes["price"], shares=request.form.get("shares"))
                # subtract cash from original table
                db.execute("UPDATE users SET cash=cash- :total WHERE id = :id", total=total, id=session["user_id"])
                
                # redirect to home
                return redirect("/")
            else:
                return apology("You do not have enough cash", 400)
            
    else:
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    # check that username does not already exist
    username = request.args.get('username')
    duplicate_user = db.execute("SELECT * FROM users WHERE username = :username",
                                username=username)
    
    # if name already taken, return False, if not return True           
    if duplicate_user:
        return jsonify(False)
        
    elif not duplicate_user:
        return jsonify(True)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # save transactions
    portfolio = db.execute("SELECT * FROM stocks WHERE id=:id ORDER BY symbol", id=session["user_id"])

    # pass portfolio data into html with necessary functions
    return render_template("history.html", portfolio=portfolio, lookup=lookup, range=range, len=len, round=round)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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
    """Get stock quote."""
    if request.method == "POST":
        # ensure fields are filled in
        if not request.form.get("symbol"):
            return apology("must provide a company symbol", 400)
            
        else:
            # retrieves data from API
            quotes = lookup(request.form.get("symbol"))
            
            # check if company exists
            if not quotes:
                return apology("Company not found", 400)
                
            # send data to quoted form
            else:
                return render_template("quoted.html", quotes=quotes, usd=usd)
        
    # User reached route via GET   
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)
            
        # ensure password was re-entered
        elif not request.form.get("confirmation"):
            return apology("must re-enter password", 400)

        # ensure password and re-enter match
        if request.form.get("confirmation") != request.form.get("password"):
            return apology("Re-entered password does not match", 400)
        else:
            hashed = generate_password_hash(request.form.get("password"))

        # check that username does not already exist
        duplicate_user = db.execute("SELECT * FROM users WHERE username = :username",
                                    username=request.form.get("username"))
        
        # if not a duplicate, add to database           
        if duplicate_user:
            return apology("Username already exists", 400)
        
        elif not duplicate_user:
            db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)",
                       username=request.form.get("username"), hash=hashed)

        # Redirect user to login page
        return redirect("/login")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    portfolio = db.execute(
        "SELECT symbol, SUM(shares) FROM stocks WHERE id=:id GROUP BY symbol ORDER BY SUM(shares) DESC", id=session["user_id"])
    
    if request.method == "POST":
        try:
            result = int(request.form.get("shares")) < 0
        except:
            return apology("Please enter a positive integer of shares to sell", 400)
            
        # if filled out correctly, begin
        # save quote data and ensure company exists
        quotes = lookup(request.form.get("symbol"))

        # calculate total cost of sale
        total = round(quotes["price"] * int(request.form.get("shares")), 2)
        
        # ensure user has enough shares
        num_shares = db.execute("SELECT SUM(shares) FROM stocks WHERE id = :id AND symbol=:symbol", 
                                id=session["user_id"], symbol=request.form.get("symbol"))[0]["SUM(shares)"]

        if num_shares >= int(request.form.get("shares")):
            # insert sale into stocks table
            db.execute("INSERT INTO stocks (id, symbol, type, price_per_share, shares) VALUES(:id, :symbol, 'sold',:cost, :shares)",
                       id=session["user_id"], symbol=request.form.get("symbol"), cost=quotes["price"]*-1, shares=int(request.form.get("shares"))*-1)
            # add cash to users table
            db.execute("UPDATE users SET cash=cash+ :total WHERE id = :id", total=total, id=session["user_id"])
            
            # redirect to home
            return redirect("/")
        else:
            return apology("You do not have enough shares", 400)
            
    else:
        return render_template("sell.html", portfolio=portfolio, range=range, len=len)


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    """adds cash"""
    if request.method == "POST":
        # cash to add
        to_add = int(request.form.get("toadd"))

        if to_add >= 0:
            # add cash to users table
            db.execute("UPDATE users SET cash=cash+ :add WHERE id = :id", add=to_add, id=session["user_id"])
            
            # redirect to home
            return redirect("/")
        
        else:
            return apology("You entered a negative amount", 403)

    else:
        return render_template("addcash.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
