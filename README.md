-----Preface and history-----

In this section I want to describe the general idea and history of the development of this project.

I've always been frustrated by any trip to the store. It was unpleasant and disgusting. One could say, "But there's delivery—why not use it?" You have to choose, scroll through product feeds. You also have to analyze products for discounts—it's inconvenient and unpleasant. I decided to implement a bot program in my favorite language, C++.

A few months ago, I had this idea in my head: what if there really was an app that would solve the problem of going to the store and choosing products? It's a brilliant idea. I'm also economically savvy and like to stock up on items that will last me until the next sale—forecasting is essential.

My first step was writing simple curl requests through the terminal, but I always got a 403 error. I thought, why not use the Python library, requests. The same error occurred—403. Then I thought, why not search for the website's API? Yes, I found it. That same day, I decided to write a few curl and requests requests with my own headers and cookies. I kept getting a captcha and couldn't get past it.
Then I looked at selenium in combination with several other Python libraries, which would help me get around the problem. The captcha didn't go away, but I realized that if I passed it, I would have access to the website's parsing. I lived like that for literally eight months, running this script every week. It was tantamount to suffering. After eight months, the site released some update that kept me stuck on the captcha and blocked me from accessing the site. I was upset. I noticed that I visited the site many times and never saw a captcha or a 403 error. I thought, why not create a script that would simply click the correct coordinates on the screen—this is the main way to bypass the captcha? My script still works this way to this day.
This incident became a good example for me that the solution is always obvious and that even the most high-profile problems can be solved with a low-level solution.

Now it's worth describing the evolution of the project itself. I tried to describe all the idioms, patterns, and tricks I've learned and am still learning in C++. This language is rich in precisely that. As I grew, I tried to find a use for these tricks. If you look at my commits, you'll notice an improvement branch where I performed code reviews. I wrote it in pure C++20.

-----Theoretical basis for forecasting-----

