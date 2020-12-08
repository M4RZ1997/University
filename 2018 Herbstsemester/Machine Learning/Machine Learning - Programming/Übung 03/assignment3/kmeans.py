import numpy as np
import time

def kmeans(X, k, max_iter=100):
    """
    Perform k-means clusering on the data X with k number of clusters.

    Args:
        X: The data to be clustered of shape [n, num_features]
        k: The number of cluster centers to be used

    Returns:
        centers: A matrix of the computed cluster centers of shape [k, num_features]
        assign: A vector of cluster assignments for each example in X of shape [n] 

    """

    centers = None
    assign = None
    i=0
    
    start = time.time()


    #######################################################################
    # TODO:                                                               #
    # Perfom k-means clustering of the input data X and store the         #
    # resulting cluster-centers as well as cluster assignments.           #
    #                                                                     #
    #######################################################################

    centers = X[np.random.randint(X.shape[0], size=k)]
    assign = np.zeros(X.shape[0], dtype=np.float64)
    distances = np.zeros([X.shape[0], k], dtype=np.float64)
    
    for i in range(max_iter):
        for i, c in enumerate(centers):
            distances[:, i] = np.linalg.norm(X - c, axis=1)
        assign = np.argmin(distances, axis=1)
        for c in range(k):
            centers[c] = np.mean(X[assign==c], 0)
    
    #######################################################################
    #                         END OF YOUR CODE                            #
    #######################################################################
    
    exec_time = time.time()-start
    print('Number of iterations: {}, Execution time: {}s'.format(i+1, exec_time))
    
    return centers, assign