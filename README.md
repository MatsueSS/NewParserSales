-----Design Solution-----

Now I want to describe the project's idea and explain why it's worth my time. I found it strange to go to stores and keep track of discounts. It's also strange to try to calculate these discounts. You might say that mobile apps already exist, but the search chore remains I told myself that I should create a program that would free me from this chore.
I'd like to give a brief introduction to what will be described below. I'll say upfront that I had to try every possible way to bypass resource blocking and CAPTCHA. I tried to find a solution using the request library from Python, but it didn't help. Then I settled on selenium.
All my calls are made using the libcurl library. Why did I decide to use this instead of Python's convenient tools? | view Python as a scripting language, and the connection with Telegram needs to be maintained constantly, so l decided to set the boundaries this way.
To access the database, I used a custom library. You can find it in the project. I used PostgreSQL as the database.
Now I want to elaborate on why I used jq in some places instead of lohmann:json. For small queries, I used jq because it's written purely in C, and a short linear search through documents is quite adequate.

-----Main Problem-----

The main problem, which l'm willing to admit, is viewing captcha when using the selenium library. Frankly, I couldn't find a suitable workaround for this. I monitored which files the site was receiving and sending. found the json I needed, but the workaround still failed. At this point, my project isn't independent and requires a solution.

-----Instructions-----

In the good_funcs module, you need to define all the functions as you'll build the project. You also need to create and populate the env file for proper operation. I have prepared cpp files for creating tables in the database, so you don't have to work too hard with this.

-----fill env-----

1 row : bot token
2 row : temp data in my env is my id
3 row : connection with db : dbname=remstocks user=matsuess password=731177889232 host=localhost port=5432 client_encoding=UTF8
4 row : temp data
5 row : last offset for telegram message

-----load-----

need use dir load
mkdir sensitive_res
mdkir res
mkdir urls

-----commands-----

recommendations - recommends popular product cards
add_card - added a card(need a card)
del_card - deleted a card(need a card)
status - show u sales
my_cards - show ur cards
forecast - forecast(need a card)

-----author words-----

One design decision is worth mentioning. I want to touch on the recommendations because it's one of the less obvious aspects of my program. The recommendations are built on the Observer pattern. It turns out that the bot is the one that modifies the observers, and the Matrix class is the observer. The recommendations are based on popular cards.