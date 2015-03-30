/*
 *  By downloading, copying, installing or using the software you agree to this license.
 *  If you do not agree to this license, do not download, install,
 *  copy or use the software.
 *  
 *  
 *  License Agreement
 *  For Open Source Computer Vision Library
 *  (3 - clause BSD License)
 *  
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met :
 *  
 *  *Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *  
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and / or other materials provided with the distribution.
 *  
 *  * Neither the names of the copyright holders nor the names of the contributors
 *  may be used to endorse or promote products derived from this software
 *  without specific prior written permission.
 *  
 *  This software is provided by the copyright holders and contributors "as is" and
 *  any express or implied warranties, including, but not limited to, the implied
 *  warranties of merchantability and fitness for a particular purpose are disclaimed.
 *  In no event shall copyright holders or contributors be liable for any direct,
 *  indirect, incidental, special, exemplary, or consequential damages
 *  (including, but not limited to, procurement of substitute goods or services;
 *  loss of use, data, or profits; or business interruption) however caused
 *  and on any theory of liability, whether in contract, strict liability,
 *  or tort(including negligence or otherwise) arising in any way out of
 *  the use of this software, even if advised of the possibility of such damage.
 */

#ifndef __OPENCV_EDGEFILTER_HPP__
#define __OPENCV_EDGEFILTER_HPP__
#ifdef __cplusplus

#include <opencv2/core.hpp>

namespace cv
{
namespace ximgproc
{

//! @addtogroup ximgproc_filters
//! @{

enum EdgeAwareFiltersList
{
    DTF_NC,
    DTF_IC,
    DTF_RF,

    GUIDED_FILTER,
    AM_FILTER
};


/** @brief Interface for realizations of Domain Transform filter.

For more details about this filter see @cite Gastal11.
 */
class CV_EXPORTS_W DTFilter : public Algorithm
{
public:

    /** @brief Produce domain transform filtering operation on source image.

    @param src filtering image with unsigned 8-bit or floating-point 32-bit depth and up to 4 channels.

    @param dst destination image.

    @param dDepth optional depth of the output image. dDepth can be set to -1, which will be equivalent
    to src.depth().
     */
    CV_WRAP virtual void filter(InputArray src, OutputArray dst, int dDepth = -1) = 0;
};

/** @brief Factory method, create instance of DTFilter and produce initialization routines.

@param guide guided image (used to build transformed distance, which describes edge structure of
guided image).

@param sigmaSpatial \f${\sigma}_H\f$ parameter in the original article, it's similar to the sigma in the
coordinate space into bilateralFilter.

@param sigmaColor \f${\sigma}_r\f$ parameter in the original article, it's similar to the sigma in the
color space into bilateralFilter.

@param mode one form three modes DTF_NC, DTF_RF and DTF_IC which corresponds to three modes for
filtering 2D signals in the article.

@param numIters optional number of iterations used for filtering, 3 is quite enough.

For more details about Domain Transform filter parameters, see the original article @cite Gastal11 and
[Domain Transform filter homepage](http://www.inf.ufrgs.br/~eslgastal/DomainTransform/).
 */
CV_EXPORTS_W
Ptr<DTFilter> createDTFilter(InputArray guide, double sigmaSpatial, double sigmaColor, int mode = DTF_NC, int numIters = 3);

/** @brief Simple one-line Domain Transform filter call. If you have multiple images to filter with the same
guided image then use DTFilter interface to avoid extra computations on initialization stage.

@param guide guided image (also called as joint image) with unsigned 8-bit or floating-point 32-bit
depth and up to 4 channels.
@param src filtering image with unsigned 8-bit or floating-point 32-bit depth and up to 4 channels.
@param dst
@param sigmaSpatial \f${\sigma}_H\f$ parameter in the original article, it's similar to the sigma in the
coordinate space into bilateralFilter.
@param sigmaColor \f${\sigma}_r\f$ parameter in the original article, it's similar to the sigma in the
color space into bilateralFilter.
@param mode one form three modes DTF_NC, DTF_RF and DTF_IC which corresponds to three modes for
filtering 2D signals in the article.
@param numIters optional number of iterations used for filtering, 3 is quite enough.
@sa bilateralFilter, guidedFilter, amFilter
 */
CV_EXPORTS_W
void dtFilter(InputArray guide, InputArray src, OutputArray dst, double sigmaSpatial, double sigmaColor, int mode = DTF_NC, int numIters = 3);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/** @brief Interface for realizations of Guided Filter.

For more details about this filter see @cite Kaiming10.
 */
class CV_EXPORTS_W GuidedFilter : public Algorithm
{
public:

    /** @brief Apply Guided Filter to the filtering image.

    @param src filtering image with any numbers of channels.

    @param dst output image.

    @param dDepth optional depth of the output image. dDepth can be set to -1, which will be equivalent
    to src.depth().
     */
    CV_WRAP virtual void filter(InputArray src, OutputArray dst, int dDepth = -1) = 0;
};

/** @brief Factory method, create instance of GuidedFilter and produce initialization routines.

@param guide guided image (or array of images) with up to 3 channels, if it have more then 3
channels then only first 3 channels will be used.

@param radius radius of Guided Filter.

@param eps regularization term of Guided Filter. \f${eps}^2\f$ is similar to the sigma in the color
space into bilateralFilter.

For more details about Guided Filter parameters, see the original article @cite Kaiming10.
 */
CV_EXPORTS_W Ptr<GuidedFilter> createGuidedFilter(InputArray guide, int radius, double eps);

/** @brief Simple one-line Guided Filter call.

If you have multiple images to filter with the same guided image then use GuidedFilter interface to
avoid extra computations on initialization stage.

@param guide guided image (or array of images) with up to 3 channels, if it have more then 3
channels then only first 3 channels will be used.

@param src filtering image with any numbers of channels.

@param dst output image.

@param radius radius of Guided Filter.

@param eps regularization term of Guided Filter. \f${eps}^2\f$ is similar to the sigma in the color
space into bilateralFilter.

@param dDepth optional depth of the output image.

@sa bilateralFilter, dtFilter, amFilter */
CV_EXPORTS_W void guidedFilter(InputArray guide, InputArray src, OutputArray dst, int radius, double eps, int dDepth = -1);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/** @brief Interface for Adaptive Manifold Filter realizations.

For more details about this filter see @cite Gastal12 and References_.

Below listed optional parameters which may be set up with Algorithm::set function.
-   member double sigma_s = 16.0
Spatial standard deviation.
-   member double sigma_r = 0.2
Color space standard deviation.
-   member int tree_height = -1
Height of the manifold tree (default = -1 : automatically computed).
-   member int num_pca_iterations = 1
Number of iterations to computed the eigenvector.
-   member bool adjust_outliers = false
Specify adjust outliers using Eq. 9 or not.
-   member bool use_RNG = true
Specify use random number generator to compute eigenvector or not.
 */
class CV_EXPORTS_W AdaptiveManifoldFilter : public Algorithm
{
public:
    /** @brief Apply high-dimensional filtering using adaptive manifolds.

    @param src filtering image with any numbers of channels.

    @param dst output image.

    @param joint optional joint (also called as guided) image with any numbers of channels.
     */
    CV_WRAP virtual void filter(InputArray src, OutputArray dst, InputArray joint = noArray()) = 0;

    CV_WRAP virtual void collectGarbage() = 0;

    CV_WRAP static Ptr<AdaptiveManifoldFilter> create();

    CV_PURE_PROPERTY(double, SigmaS)
    CV_PURE_PROPERTY(double, SigmaR)
    CV_PURE_PROPERTY(int, TreeHeight)
    CV_PURE_PROPERTY(int, PCAIterations)
    CV_PURE_PROPERTY(bool, AdjustOutliers)
    CV_PURE_PROPERTY(bool, UseRNG)
};

/** @brief Factory method, create instance of AdaptiveManifoldFilter and produce some initialization routines.

@param sigma_s spatial standard deviation.

@param sigma_r color space standard deviation, it is similar to the sigma in the color space into
bilateralFilter.

@param adjust_outliers optional, specify perform outliers adjust operation or not, (Eq. 9) in the
original paper.

For more details about Adaptive Manifold Filter parameters, see the original article @cite Gastal12.

@note Joint images with CV_8U and CV_16U depth converted to images with CV_32F depth and [0; 1]
color range before processing. Hence color space sigma sigma_r must be in [0; 1] range, unlike same
sigmas in bilateralFilter and dtFilter functions.
*/
CV_EXPORTS_W Ptr<AdaptiveManifoldFilter> createAMFilter(double sigma_s, double sigma_r, bool adjust_outliers = false);

/** @brief Simple one-line Adaptive Manifold Filter call.

@param joint joint (also called as guided) image or array of images with any numbers of channels.

@param src filtering image with any numbers of channels.

@param dst output image.

@param sigma_s spatial standard deviation.

@param sigma_r color space standard deviation, it is similar to the sigma in the color space into
bilateralFilter.

@param adjust_outliers optional, specify perform outliers adjust operation or not, (Eq. 9) in the
original paper.

@note Joint images with CV_8U and CV_16U depth converted to images with CV_32F depth and [0; 1]
color range before processing. Hence color space sigma sigma_r must be in [0; 1] range, unlike same
sigmas in bilateralFilter and dtFilter functions. @sa bilateralFilter, dtFilter, guidedFilter
*/
CV_EXPORTS_W void amFilter(InputArray joint, InputArray src, OutputArray dst, double sigma_s, double sigma_r, bool adjust_outliers = false);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/** @brief Applies the joint bilateral filter to an image.

@param joint Joint 8-bit or floating-point, 1-channel or 3-channel image.

@param src Source 8-bit or floating-point, 1-channel or 3-channel image with the same depth as joint
image.

@param dst Destination image of the same size and type as src .

@param d Diameter of each pixel neighborhood that is used during filtering. If it is non-positive,
it is computed from sigmaSpace .

@param sigmaColor Filter sigma in the color space. A larger value of the parameter means that
farther colors within the pixel neighborhood (see sigmaSpace ) will be mixed together, resulting in
larger areas of semi-equal color.

@param sigmaSpace Filter sigma in the coordinate space. A larger value of the parameter means that
farther pixels will influence each other as long as their colors are close enough (see sigmaColor ).
When d\>0 , it specifies the neighborhood size regardless of sigmaSpace . Otherwise, d is
proportional to sigmaSpace .

@param borderType

@note bilateralFilter and jointBilateralFilter use L1 norm to compute difference between colors.

@sa bilateralFilter, amFilter
*/
CV_EXPORTS_W
void jointBilateralFilter(InputArray joint, InputArray src, OutputArray dst, int d, double sigmaColor, double sigmaSpace, int borderType = BORDER_DEFAULT);

//! @}

}
}
#endif
#endif