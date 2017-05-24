[Описание на русском языке](README.ru.md)

# The Tengu.

<p style="float: left; " >
    <img src="images/Unit_ills_full_40083.png" width="256" /> 
</p> 

The **Tengu** ("heavenly dog") are a kind of mythical creatures in Japanese folklore. They helped people (sometimes).
They saw everything and everyone knew (always). In my case this is not more than a nice word only. The purpose of 
this project is to get to know more closely the technologies of artificial intelligence using the example of UAV. 
Often the people mixes two different concepts, calling the term UAV of simply "unmanned" aircraft  (the lack of a 
pilot on board). At the same time, the aircraft is piloted by a person from the ground via the radio channel with 
the help of camera / television. This project uses somewhat different understanding of the term UAV, the namely a 
full autonomy. The UAV must make its own decision by themself, as much as possible.

If we will say some clever words, we get a multiagent computational model of consciousness ( according to Steven 
Pinker ). There is not and it is not expected yet (at present) a neural networks in this project. But there is an attempt 
to divide the decision-making into many small steps. Each of which is executed by his own program (the agent).
Agents can communicate with each other under certain conditions (when they "see" each other, at a certain distance).

# The contents of project

| The directory | Description |
| --- | --- |
| [LoRedis](https://github.com/unclesal/tengu/tree/master/loredis) | C++ / Qt wrapper for Redis.IO, a mechanism for exchanging messages between agents |
| [X-Tengu](https://github.com/unclesal/tengu/tree/master/xtengu) | The X-Plane plugin for simulation and modeling of aircraft's behavior in the air. |



