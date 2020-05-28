# SecureDeepLearning
### compile using GCC wrapper of obliv-c
/path/to/oblivcc secure_deep_learning.c secure_deep_learning.oc -I .
### run: party 1 holds the deep learning algirithm's parameters learned for detecting vagueness, party 2 holds the input to be tested for vagueness.
cycle './a.out 1 | ./a.out 2'
### Result output should be 0 (input word is not vague) or 1 (input word is vague). 
