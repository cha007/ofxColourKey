ofxColourKey
============

Chroma keying for openframeworks:

I use a 3D distance key algorithm for creating a matte. The algorithm compares the keying color (i.e. blue or green, can be set) to each pixel. It calculates the distance between the 2 values in 3D space (=RGB is understood as 3D space). This way, we receive a distance value for each pixel which represents the alpha value.

I added 2 values to optimize the results, a threshold and a tolernace value. Both can be tweeked for your footage.

In addition, there is a key colour finder that tries to automatically determin the used keying colour.


TO DO:
I like to add a spill removal function.
