-----Abstract-----

This project presents a Telegram bot that predicts product discounts
using geometric distribution and Markov chains. The bot collects data
by bypassing captcha through OS-level mouse emulation (ydotool).
Statistical methods include Chi-square test for independence,
BIC for model selection, and ROC-AUC (0.85) for quality assessment.
The system serves real users and handles 6500+ products.

-----Preface and history-----

In this section I want to describe the general idea and history of the development of this project.

I've always been frustrated by any trip to the store. It was unpleasant and disgusting. One could say, "But there's delivery—why not use it?" You have to choose, scroll through product feeds. You also have to analyze products for discounts—it's inconvenient and unpleasant. I decided to implement a bot program in my favorite language, C++.

A few months ago, I had this idea in my head: what if there really was an app that would solve the problem of going to the store and choosing products? It's a brilliant idea. I'm also economically savvy and like to stock up on items that will last me until the next sale—forecasting is essential.

My first step was writing simple curl requests through the terminal, but I always got a 403 error. I thought, why not use the Python library, requests. The same error occurred—403. Then I thought, why not search for the website's API? Yes, I found it. That same day, I decided to write a few curl and requests requests with my own headers and cookies. I kept getting a captcha and couldn't get past it.
Then I looked at selenium in combination with several other Python libraries, which would help me get around the problem. The captcha didn't go away, but I realized that if I passed it, I would have access to the website's parsing. I lived like that for literally eight months, running this script every week. It was tantamount to suffering. After eight months, the site released some update that kept me stuck on the captcha and blocked me from accessing the site. I was upset. I noticed that I visited the site many times and never saw a captcha or a 403 error. I thought, why not create a script that would simply click the correct coordinates on the screen—this is the main way to bypass the captcha? My script still works this way to this day.
This incident became a good example for me that the solution is always obvious and that even the most high-profile problems can be solved with a low-level solution.

Now it's worth describing the evolution of the project itself. I tried to describe all the idioms, patterns, and tricks I've learned and am still learning in C++. This language is rich in precisely that. As I grew, I tried to find a use for these tricks. If you look at my commits, you'll notice an improvement branch where I performed code reviews. I wrote it in pure C++20.

-----Theoretical basis for forecasting-----

It's worth justifying the project, because many things seem illogical at first glance. Perhaps the reader simply isn't familiar with statistics, and these points deserve clarification.
Before moving on to the discussion, I want to introduce the Law of Large Numbers at the head of this section. This law states that with a large sample, the statistical mean of that sample will converge to the mathematical expectation. This is the most important theorem to keep in mind when working with statistics.
Probability theory has a section on geometric distribution. The essence of this important model is that trials are conducted until a success is achieved, counting the number of failures. Let the probability of success be p, then the probability of success within three trials is (1-p)^2 * p. It's important here that the probability p is essentially constant. Also, all trials are independent of each other.
Now, back to our task: while monitoring discounts, I noticed that the discounts are updated once a week, so collecting data once a week is sufficient. I chose Saturday for this day. Data is collected every Saturday. Next, we need to define what we'll consider a trial and a random variable. Here, the random variable is the week number when I observe a discount. The choice of week number is random. And I'm going to predict the next week's discount. This doesn't refute the idea that the discount is deterministic. But it's crucial to verify the dependence of the discount on the week number and on previous results.
To test whether the discount depends on the week number, I chose the Chi-Square test, which allows, for a given significance coefficient, to determine whether the hypothesis should be rejected. I used a coefficient of 0.05, since a Type I error is less of a concern than a Type II error.

I wrote a module that calculates the criterion for all unique products. I found that only 17 products out of 6,500 depend on the week number. This suggests that the geometric distribution can be applied to 6,500-17 products without introducing an error. After writing the first model and getting my app up and running significantly, I realized I shouldn't stop at just one model and decided to consider first- and second-order Markov chains. Now, forecasting worked like this: the Bayesian Information Criterion was calculated first, and then a probability was selected from three models. I'll say upfront that in more than 90% of cases, the model with a geometric distribution was selected—this indicates that the data is independent of previous outcomes, confirming my choice of this distribution.
I tried various logistic regression tests in Python, and it often yielded a high coefficient, indicating its unsuitability.
The most interesting part was calculating the ROC-AUC for the model with a geometric distribution. And yes, I wrote code that does this for all products. On average, for products with a large number of trials, the test showed an average of 0.85, confirming a good model selection. If this test were calculated using weights for a model made up of three other models, the result would be under 0.9, indicating excellent results. Thus, the choice of this distribution is fully justified theoretically and practically.

But there's one problem: there are products that sell during specific seasons. For example, tangerines sell well in winter, apples in autumn, and so on. My point is that some products are seasonally dependent. This is a very important observation. I've solved it this way: for all products where the chi-square test showed a correlation, we'll create a sample for specific seasons. That is, we'll be looking at specific seasons, not the entire sample.

-----used libraries and tools-----

ydotool, Firefox
C++: boost/math, libpq, libcurl, nlohmann/json
python: subprocess

-----Architectural solutions-----

In this section I want to share the selection of algorithms, data structures and patterns

I spend a lot of time solving problems on leetcode, where I learn a lot of good solutions. It was from there that I borrowed the idea of ​​a prefix tree, which I implemented for convenient user input. I also used dfs and several other interesting algorithms.
My project places a lot of emphasis on the Strategy and Factory patterns—these are my two favorite patterns because they make it very easy to extend code and create new ones. If you haven't used them, you should.
I've been heavily favoring hash maps and sets due to their speed. But they take up a lot of memory. This is precisely the point of one of the most important optimizations in my project. Previously, I stored all user preferences as strings. A potential string could be anywhere from 5 to 50 characters long. That's a lot. Moreover, these strings could be repeated by other users, which is also wasteful. I decided to write a module that could convert strings into numbers. Thus, I gave each unique product card its own index, starting with 1. This allowed me to store preferences as numbers rather than strings, which significantly sped up the work.

-----Results-----

- 6500+ products monitored weekly
- 17 products show seasonal dependency
- ROC-AUC: 0.85 (very good)
- BIC selects geometric distribution in 90%+ cases
- Real users actively using the bot

-----Future Work-----

- Rewrite Python parser in C++ (eliminate system() calls)
- Writing a cache for many queries and results
- Implement a factory for creating commands for the bot instead of a set of commands
- Write tests for most modules
- Implement seasonal forecasting (not done due to lack of data)
- Adding logistic regression in the future as a new model
- Implement separate Markov chain forecasting for products with explicit week number dependency
- Change user IDs from string data type to uint32_t


-----Literature reference-----

Bjarne Stroustrup - Programming -- Principles and Practice Using C++ (3rd Edition)
Scott Meyers - Effective C++
Richard Stevens - Unix: network programming 
E.S. Venttsel - Probability Theory
Norris - Markov Chains

-----Acknowledgments-----

I want to thank my parents, because they were the first ones who could soberly evaluate my project and say that this project is really interesting.
I want to express my gratitude to my project-based teacher, because he was the first person who made me question my theoretical decisions. He hinted at the contradictions that would arise in the future.
I also want to express my gratitude to my supervisor, Elena Arutyunyan Babkenovna. She listened to every decision I made and provided analysis that determined the direction of my project.
When I told people about my project, they told me it couldn't possibly be that simple. Marketing involves a ton of parameters I don't consider. And indeed, I look at it from an independent perspective and am uninterested in the internal processes. The problem itself is modeled by the distribution I chose. This choice is strongly supported by the Bayesian information coefficient and the ROC-AUC test.