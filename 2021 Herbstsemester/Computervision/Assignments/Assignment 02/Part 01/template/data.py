import os

import numpy as np
from PIL import Image


def load_merton_college_data(folder):
    im1 = np.array(Image.open(os.path.join(folder, 'images/001.jpg')))
    im2 = np.array(Image.open(os.path.join(folder, 'images/002.jpg')))

    # load 2D points for each view to a list
    points_2d = [np.loadtxt(os.path.join(folder, f'2D/00{i + 1:d}.corners')).T for i in range(3)]

    # load 3D points
    points_3d = np.loadtxt(os.path.join(folder, '3D/p3d')).T

    # load correspondences
    corr = np.genfromtxt(os.path.join(folder, '2D/nview-corners'), dtype=int, missing_values='*')

    # create cameras
    P = [np.loadtxt(os.path.join(folder, f'2D/00{i + 1:d}.P')) for i in range(3)]

    return im1, im2, points_2d, points_3d, corr, P


def load_homogeneous_coordinates(data_dir):
    im1, im2, points_2d, _, corr, _ = load_merton_college_data(data_dir)

    # index for visible points in first two views
    ndx = (corr[:, 0] >= 0) & (corr[:, 1] >= 0)

    # get coordinates and make homogeneous
    x1 = points_2d[0][:, corr[ndx, 0]]
    x1 = np.vstack((x1, np.ones(x1.shape[1])))
    x2 = points_2d[1][:, corr[ndx, 1]]
    x2 = np.vstack((x2, np.ones(x2.shape[1])))

    return im1, im2, x1, x2
