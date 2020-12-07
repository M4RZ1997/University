#! /usr/bin/env python3
# -*- coding: utf-8 -*-
'''
module for performing the
basic steps of a jpeg encoding.
'''


import numpy as np
import matplotlib.pyplot as plt
import scipy as sp
import cv2

from imageio import imread
from scipy import signal
from math import log10, sqrt
from skimage.metrics import structural_similarity as ssim


# quantization matrix
qt_d = np.array([[16, 11, 10, 16,  24,  40,  51, 61],
               [12, 12, 14, 19,  26,  58,  60, 55],
               [14, 13, 16, 24,  40,  57,  69, 56],
               [14, 17, 22, 29,  51,  87,  80, 62],
               [18, 22, 37, 56,  68, 109, 103, 77],
               [24, 35, 55, 64,  81, 104, 113, 92],
               [49, 64, 78, 87, 103, 121, 120, 101],
               [72, 92, 95, 98, 112, 100, 103, 99], ])

#qt_y
qt_y = np.array([[5,3,4,7,11,16,24,34],
               [3,4,4,6,8,12,18,25],
               [4,4,8,9,11,15,20,28],
               [7,6,9,14,16,20,26,33],
               [11,8,11,16,26,28,34,42],
               [16,12,15,20,28,41,46,54],
               [24,18,20,26,34,46,63,71],
               [34,25,28,33,42,54,71,95], ])
#qt_cr
qt_cr = np.array([[7,9,18,28,43,63,91,128],
               [9,8,17,23,33,48,68,94],
               [18,17,31,34,43,58,78,105],
               [28,23,34,55,63,77,98,126],
               [43,33,43,63,98,108,128,157],
               [63,48,58,77,108,154,174,204],
               [91,68,78,98,128,174,239,255],
               [128,94,105,126,157,204,255,255], ])
#qt_cb
qt = np.array([[14,18,46,71,109,161,232,255],
               [18,17,43,59,85,122,173,240],
               [46,43,80,87,111,148,200,255],
               [71,59,87,142,160,196,249,255],
               [109,85,111,160,251,255,255,255],
               [161,122,148,196,255,255,255,255],
               [232,173,200,249,255,255,255,255],
               [255,240,255,255,255,255,255,255], ])


def psnr(img1, img2):
    mse = np.mean((img1 - img2) ** 2)
    if (mse == 0):
        return 100
    PIXEL_MAX = 255.0
    return 20 * log10(PIXEL_MAX / sqrt(mse))


def dct2(a):
    '''2d discrete cosine transform
    '''

    return sp.fft.dct(sp.fft.dct(a, axis=0, norm='ortho'), axis=1, norm='ortho')


def idct2(a):
    '''inverse 2d discrete cosine transform
    '''

    return sp.fft.idct(sp.fft.idct(a, axis=0, norm='ortho'), axis=1, norm='ortho')


def quantization(im, plot=True):
    '''function to perform dct on
       8x8 blocks and quantize the
       components
    '''

    global qt
    imsize = im.shape
    dct = np.zeros(imsize)  # make a null matrix with the same dimensions
    dct_whole = np.zeros(imsize)

    # Do 8x8 DCT on image (in-place)
    for i in np.r_[:imsize[0]:8]:  # process 8x8 blocks
        for j in np.r_[:imsize[1]:8]:
            # multiply by the quantization matrix
            block = np.divide(dct2(im[i:(i+8), j:(j+8)]), qt)
            # store in the dct
            dct[i:(i+8), j:(j+8)] = np.round(block)
            dct_whole[i:(i+8), j:(j+8)] = dct2(im[i:(i+8), j:(j+8)])
    if (plot):
        pos = 128

        # Extract a block from image
        plt.figure()
        plt.imshow(im[pos:pos+8, pos:pos+8], cmap='gray')
        plt.savefig("image_block.png", bbox_inches='tight')

        # Display the dct of that block
        plt.figure()
        plt.imshow(np.multiply(dct[pos:pos+8, pos:pos+8], qt), cmap='gray',
                   vmax=np.max(dct)*0.01, vmin=0, extent=[0, np.pi, np.pi, 0])
        plt.savefig("dct_block.png", bbox_inches='tight')

        # Display entire DCT
        plt.figure()
        plt.imshow(dct_whole, cmap='gray', vmax=np.max(dct)*0.01, vmin=0)
        plt.savefig('dct_whole.png', bbox_inches='tight')

    return dct


def dequantization(dct, plot=True):
    '''function to perform inverse dct
       and multiply coeficients by the
       quantization factor.
    '''

    global qt
    imsize = dct.shape
    im_dct = np.zeros(imsize)  # matrix for reconstruction
    idct_whole = np.zeros(imsize)

    for i in np.r_[:imsize[0]:8]:
        for j in np.r_[:imsize[1]:8]:
            block = np.multiply(dct[i:(i+8), j:(j+8)], qt)
            im_dct[i:(i+8), j:(j+8)] = idct2(block)
            idct_whole[i:(i+8), j:(j+8)] = block
    if (plot):
        pos = 128

        # Display the dct of that block
        plt.figure()
        plt.imshow(np.multiply(im_dct[pos:pos+8, pos:pos+8], qt), cmap='gray',
                   vmax=np.max(dct)*0.01, vmin=0, extent=[0, np.pi, np.pi, 0])
        plt.savefig("dct_block2.png", bbox_inches='tight')

        plt.figure()
        plt.imshow(idct_whole, cmap='gray',
                   vmax=np.max(idct_whole)*0.01, vmin=0)
        plt.savefig('idct_whole.png', bbox_inches='tight')

    return im_dct


def main():
    im = cv2.imread('doge.jpg', 0)  # input image in grayscale
    
    f = plt.figure()
    plt.imshow(im, cmap='gray')
    plt.savefig("lena_gray.png", bbox_inches='tight')

    dct = quantization(im)

    # Display entire DCT
    plt.figure()
    plt.imshow(dct, cmap='gray', vmax=np.max(dct)*0.01, vmin=0)
    plt.title('8x8 DCTs of the image')

    im_dct = dequantization(dct)

    # Display entire DCT
    plt.figure()
    plt.imshow(im_dct, cmap='gray', vmax=np.max(dct)*0.01, vmin=0)
    plt.title('8x8 DCTs of the image')
    im_dct = im_dct.astype('int')
    im = im.astype('int')

    plt.figure()
    plt.imshow(np.hstack((im, im_dct)), cmap='gray')
    plt.savefig('comp.png', bbox_inches='tight')

    print("SSIM of the reconstructed image", ssim(im, im_dct, data_range=im_dct.max() - im_dct.min()))
    print("PSNR of reconstructed image", psnr(im, im_dct))


if __name__ == '__main__':
    main()
