#lda

LDA code for Biometric Creative Inquiry

##File descriptions


###CreateDatabase:
- Aligns a set of face images
- Outputs a matrix where each column is a linearized image

###example:
- driver that runs CreateDatabase

###FisherFaceCore:
?

###grayscale:
- Converts a PPM image to grayscale

###ppm:
- Contains all functions dealing with a PPM image which include - constructor,
destructor, read header, and convert from P3 -> P6 (changing the magic number)

###Recognition:
- This function compares two faces by projecting the images into facespace and
measures the Euclidean distance between them.

###Recognition_sbh:
?
