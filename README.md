# Pong AI

Clicking the picture goes to the demo
[![Pong AI vs. AI Demo](https://img.youtube.com/vi/Zic6aHsQW18/0.jpg)](https://www.youtube.com/watch?v=Zic6aHsQW18)

## The goal of the project
I wanted to experiment with reinforcement learning in a game environment. First, I was thinking about implementing
this in to the chess I earlier made in Python, but I thought it was more productive to do something different and
this time in C++. When I tried to think of a simple game, the pong game first came to mind.

## The implementation
I first created the game environment and the rules and states of the game. I tried to apply the Q-learning algorithm
from a university machine learning course that was made in Python, but it was apparent from the first results that
this method was not suitable for this environment. When I chained the actions together from the start of the serve
to the end of the serve, first signs of actual learning started to be visible.

## Results and lessons learned
A key lesson here was that the size of the Q-table even in this relatively simple environment quickly becomes huge
(in the tens of millions here) and therefore an integral part of the project is to reduce the possible states by
approximations. Based on some experimenting, the parameters for the algorithm are currently well chosen. I used multiple different tricks to get the agents learning very quickly.
The training could still be run for longer, but the training starts to take a very long time as the performance is closer to perfect.
