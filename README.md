-----Abstract-----

This project presents a Telegram bot that predicts product discounts
using geometric distribution, logistic regression and catboost. The bot collects data
by bypassing captcha through OS-level mouse emulation (ydotool).
Statistical methods include Chi-square test for independence,
BIC for model selection, and ROC-AUC for quality assessment.
The system serves real users and handles 13000+ products.

-----Preface and history-----

In this section I want to describe the general idea and history of the development of this project.

I've always been frustrated by any trip to the store. It was unpleasant and disgusting. One could say, "But there's delivery—why not use it?" You have to choose, scroll through product feeds. You also have to analyze products for discounts—it's inconvenient and unpleasant. I decided to implement a bot program in my favorite language, C++.

A few months ago, I had this idea in my head: what if there really was an app that would solve the problem of going to the store and choosing products? It's a brilliant idea. I'm also economically savvy and like to stock up on items that will last me until the next sale—forecasting is essential.

My first step was writing simple curl requests through the terminal, but I always got a 403 error. I thought, why not use the Python library, requests. The same error occurred—403. Then I thought, why not search for the website's API? Yes, I found it. That same day, I decided to write a few curl and requests requests with my own headers and cookies. I kept getting a captcha and couldn't get past it.
Then I looked at selenium in combination with several other Python libraries, which would help me get around the problem. The captcha didn't go away, but I realized that if I passed it, I would have access to the website's parsing. I lived like that for literally eight months, running this script every week. It was tantamount to suffering. After eight months, the site released some update that kept me stuck on the captcha and blocked me from accessing the site. I was upset. I noticed that I visited the site many times and never saw a captcha or a 403 error. I thought, why not create a script that would simply click the correct coordinates on the screen—this is the main way to bypass the captcha? My script still works this way to this day.
This incident became a good example for me that the solution is always obvious and that even the most high-profile problems can be solved with a low-level solution.

Now it's worth describing the evolution of the project itself. I tried to describe all the idioms, patterns, and tricks I've learned and am still learning in C++. This language is rich in precisely that. As I grew, I tried to find a use for these tricks. If you look at my commits, you'll notice an improvement branch where I performed code reviews. I wrote it in pure C++20.

-----Theoretical basis for forecasting-----

It is worth justifying the project, as many aspects of it might seem counterintuitive at first glance. The reader may simply be unfamiliar with the relevant statistics, so these points require clarification. Before proceeding with the discussion, I would like to introduce the law of large numbers, which serves as the starting point for this section. This law states that with a large sample size, the sample mean tends to converge toward the expected value. This is a crucial theorem to keep in mind when working with statistics.

Probability theory includes the concept of the geometric distribution. The essence of this important model is that trials are conducted until a success is achieved, while the number of failures is counted. Let the probability of success be *p*; then the probability of achieving success within three trials is (1-*p*)² * *p*. It is important to note that the probability *p* remains essentially constant, and all trials are independent of one another.
Returning to our problem: while tracking discounts, I noticed that they are updated weekly, so collecting data at that frequency is sufficient. I chose Saturday for this purpose; data collection takes place every Saturday. Next, we must define exactly what constitutes a trial and what the random variable is. In this case, the random variable is the week number during which I record the discount. The week number is selected at random. My goal is to forecast the discount for the following week. This does not contradict the assumption that the discount amount is determined by deterministic factors. However, it is crucial to verify whether the discount depends on the week number or on previous results.

To test for a dependency between the discount and the week number, I chose the chi-square test, which allows one to determine whether to reject a hypothesis at a given significance level. I used a significance level of 0.05, as a Type I error is less critical in this context than a Type II error. I wrote a software module to calculate this criterion for all unique products. It turned out that only 17 out of 13,000 products show a dependency on the week number. This leads to the conclusion that a geometric distribution model can be applied to the remaining products (13,000 minus 17) without introducing error. Where do things stand now? I have a discount forecasting model based on a single parameter; consequently, with a large number of trials, the ROC-AUC would degenerate to 0.5. I decided to incorporate logistic regression, as it is ideally suited for this scenario—where a forecast ranging from 0 to 1 is required. Although this type of regression typically functions as a classification model, there is nothing preventing its use as a standard regression model. I implemented the model's features and ran it using liblinear.h. However, a problem arises: selecting all features results in an excessively high BIC, which is undesirable. I need an optimal way to select features. By starting with the feature most highly correlated with the sample and then adding the least correlated features until the BIC begins to rise, I can obtain a model with optimally selected parameters; this serves as the first approximation method in this study, relying on fundamental correlation properties. Next, I incorporated boosting—specifically, CatBoost; the underlying concept of this boosting method is what matters most to me. The model is generated using Python, while reading and interacting with the model is handled via C++. All features have been implemented, though the interaction logic is currently under development.

However, there is one issue: some products are sold only during specific seasons. For instance, tangerines sell well in winter, apples in autumn, and so on. The key point is that sales of certain products are seasonal. This is a crucial observation. I addressed this challenge as follows: for all products where the chi-square test indicated a correlation, we create samples tied to specific seasons. In other words, we analyze individual seasons rather than the entire dataset. Incidentally, boosting algorithms handle these situations perfectly.

For future steps, I plan to apply boosting models to the entire dataset—this is the initial study using boosting. Next, I intend to create clusters based on product categories and apply boosting models to them. Afterward, I plan to create further clusters based on price dynamics and apply boosting models to those as well; this will allow me to identify the best-performing model based on ROC-AUC.

-----used libraries and tools-----

ydotool, Firefox
C++: boost/math, libpq, libcurl, nlohmann/json, liblinear, catboost
python: subprocess

-----Architectural Decisions-----

In this section, I would like to discuss the algorithms, data structures, and design patterns I selected.

I spend a lot of time solving problems on LeetCode, where I discover many effective approaches. That is where I picked up the idea of ​​using a prefix tree (trie), which I implemented to efficiently handle user input. I also employed the Depth-First Search (DFS) algorithm and several other interesting algorithms.
My project places special emphasis on the Strategy and Factory patterns—these are my favorites because they significantly simplify code extensibility and the addition of new functionality. If you haven't used them yet, I highly recommend doing so.
I often favor hash tables and sets due to their high performance. However, they consume a significant amount of memory. This aspect was key to one of the project's most important optimizations. Previously, I stored all user preferences as strings. These strings ranged from 5 to 50 characters in length—quite substantial. Furthermore, the same strings appeared across different users, leading to data redundancy. I decided to write a module to convert strings into numbers. As a result, each unique product card was assigned an index (starting at 1). This allowed preferences to be stored in a numeric format rather than as strings, which significantly boosted system performance.

I should also highlight my use of the CRTP (Curiously Recurring Template Pattern). It enables static polymorphism instead of dynamic polymorphism. This is useful when the implementation is determined at compile time; for instance, features for boosting models and logistic regression are known beforehand, making compile-time selection the ideal choice. I also enhanced the implementation using variadic templates.

-----Results-----

- ​​Weekly monitoring of over 13,000 products
- Seasonal patterns identified in 17 products
- ROC-AUC: 0.91 (an excellent result); the best metric was selected from among all models (geometric distribution, logistic regression, boosting algorithms)
- BIC criterion favors the geometric distribution model in over 90% of cases (though this is currently based on a limited number of data points)
- Active use of the bot by real users
- Comprehensive analysis of the discounts themselves: models were developed to handle individual products, the entire product dataset, and product clusters (with cluster-based forecasting planned for the future). Thus, I examined discount formation from every angle.

-----Future Work-----

- ​​Implementing caching for multiple requests and results
- Writing tests for most modules
- Implementing seasonal forecasting (not completed due to a lack of data)
- Adding clusters for catboost


-----Literature reference-----

Bjarne Stroustrup - Programming -- Principles and Practice Using C++ (3rd Edition)
Scott Meyers - Effective C++
Richard Stevens - Unix: network programming 
E.S. Venttsel - Probability Theory

-----Acknowledgments-----

I would like to thank my parents; they were the first to objectively evaluate my project and recognize that it was truly interesting.
I wish to express my gratitude to the project supervisor, who was the first to prompt me to question my initial theoretical choices and pointed out potential contradictions that could arise later on.
I also want to thank my academic advisor, Elena Babkenovna Arutyunyan. She paid close attention to every decision I made and conducted an analysis that shaped the direction of the project's development.
When I spoke to others about my project, they often remarked that it couldn't possibly be that simple, noting that marketing involves numerous variables I had not taken into account. Indeed, I was viewing the situation from an external perspective without delving into internal processes. However, the problem itself is effectively described by the distribution model I selected—a choice robustly supported by the Bayesian Information Criterion (BIC) and the ROC-AUC metric.
I am grateful to Sergey Vyacheslavovich Drozhzhin for his recommendations regarding model selection and for his brief analysis of my project.
I am grateful to Nadezhda Nikolaevna Zolnikova for the knowledge she shared regarding regression and boosting techniques.