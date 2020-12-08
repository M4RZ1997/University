import numpy as np

def getEigenImages(images, basis):
    
    eigen_coefficients = None
    reconstruction = None
    
    #######################################################################
    # TODO:                                                               #
    #      Compute eigen coefficients and reconstruct the faces from      #
    #      coefficients                                                   #
    # Input:  images - images to compress                                 #
    #         basis - eigenbasis for compression                          #
    # Output: eigen_coefficients - coefficients corresponding to each     #
    #                              eigenvecvtor                           #
    #         reconstruction - compressed images                          #
    #######################################################################
    eigen_coefficients = np.matmul(basis.T, images.T)
    reconstruction = np.matmul(eigen_coefficients, images)        
    
    #######################################################################
    #                         END OF YOUR CODE                            #
    #######################################################################

    return eigen_coefficients, reconstruction
