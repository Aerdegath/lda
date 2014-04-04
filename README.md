#lda

LDA code for Biometric Creative Inquiry

##File descriptions


###example:
- driver: runs CreateDatabase, FisherfaceCore, Recognition in that order

###CreateDatabase:
- Aligns a set of face images
- Outputs a matrix where each column is a linearized image

###FisherfaceCore:
- projects faces onto a facespace

###Recognition:
- This function compares two faces by projecting the images into facespace and
measures the Euclidean distance between them.

###grayscale:
- Converts a PPM image to grayscale

###ppm:
- Contains all functions dealing with a PPM image which include - constructor,
destructor, read header, and convert from P3 -> P6 (changing the magic number)
