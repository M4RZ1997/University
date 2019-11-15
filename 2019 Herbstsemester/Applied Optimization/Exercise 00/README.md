## Execercise 00
#### Group Marcel Zauder, Elias Wipfli and Lorenzo Wipfli

Task 1:

2D function, nonConvex and ND function:
The implementation of the functions was straight-forward (the formulas were given, we just needed to code it correctly).

For the results see the 2dquadratic and nonconvex pictures and csv, which look like the pictures shown in the exercise lecture.


Task 2:
In the GridSearch.hh file is the _n_grid = 10. So we search always 10 steps per dimension.

For the 2d-Search we initialized a first minimum (we just take the lower bound x as value) and the step.
For the step we take the values in the lower and upper bound vectors (they are the same per dimension, because the lower/upper bound x vectors are initialized with the specified same value, so lower bound x for example is (-10,-10) and upper bound x (10,10)), then we substract them and divide the result with (_n_grid - 1).
With this we have a uniform distance for every gridpoint.

We had to make a distinction for noConvex and 2D functions. In both cases we go through every dimension with the step distance, except if we have found in the convex function a minimum in the dimension, then we can skip to the next dimension. We do this because a convex function doesn't have 2 minimums, so if we have reached a local minimal point and in the next step it is higher, then we know that the following steps will also be higher.

Like the above functions, where we made a grid search in 2 dimension, we made the function so it can search n dimensions. The only problem is that the 2d functions are really fast but the more dimensions are added the longer it takes to find the minimum. We wanted to see the minimum that was found, so we inserted in the printed it out.

Example:
-100 lower bound, 100 upper bound, grid 100, dimension 10 -> 0 sec
-100 lower bound, 100 upper bound, grid 100, dimension 100 -> 0 sec
-100 lower bound, 100 upper bound, grid 100, dimension 1000 -> 0.002 sec
-100 lower bound, 100 upper bound, grid 100, dimension 10000 -> 0.294 sec
-100 lower bound, 100 upper bound, grid 100, dimension 100000 -> 94.944 sec


