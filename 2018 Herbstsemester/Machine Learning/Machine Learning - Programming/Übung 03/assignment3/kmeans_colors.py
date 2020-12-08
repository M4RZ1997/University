from sklearn.cluster import KMeans
import numpy as np


def kmeans_colors(img, k, max_iter=100):
    """
    Performs k-means clusering on the pixel values of an image.
    Used for color-quantization/compression.

    Args:
        img: The input color image of shape [h, w, 3]
        k: The number of color clusters to be computed

    Returns:
        img_cl:  The color quantized image of shape [h, w, 3]

    """

    img_cl = None

    #######################################################################
    # TODO:                                                               #
    # Perfom k-means clustering of on the pixel values of the image img.  #
    #######################################################################
    
    shapedimg = img.reshape(img.shape[0] * img.shape[1], img.shape[2])
    kmeans = KMeans(n_clusters = k, max_iter = max_iter).fit(shapedimg)
    
    for i in range((shapedimg.shape[0])):
        shapedimg[i] = kmeans.cluster_centers_[kmeans.labels_[i]]
        
    img_cl = shapedimg.reshape(img.shape[0], img.shape[1], img.shape[2])
    
    
    #######################################################################
    #                         END OF YOUR CODE                            #
    #######################################################################

    return img_cl