/**
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
* IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
**/

// -----------------------------------------------------------

// ===========================================================
// INCLUDES
// ===========================================================

// HEADER
#include "main.hpp"

// ===========================================================
// TYPES
// ===========================================================

/**
  * @brief
  * Area-Component struct.
**/
struct AreaCmp
{

    // -----------------------------------------------------------

    // ===========================================================
    // CONSTANTS & FIELDS
    // ===========================================================

    /** Areas (pointer to collection of Points). **/
    const std::vector<float>* mAreas;

    // ===========================================================
    // CONSTRUCTORS, DESTRUCTOR & METHODS & OPERATORS
    // ===========================================================

    AreaCmp( const std::vector<float>& pAreas )
        : mAreas( &pAreas )
    {
    }

    bool operator()(int a, int b) const
    {
        const std::vector<float>& areas_ = *mAreas;
        return areas_[a] > areas_[b];
    }

    // -----------------------------------------------------------

};

// ===========================================================
// FIELDS
// ===========================================================

/** Thresholds. **/
int minThresh = 255, maxThrash = 63, apertureSize = 3;

/** Matrices. **/
cv::Mat img_gry, img_bin;

// ===========================================================
// METHODS
// ===========================================================

void onTrackbar( int, void* )
{
    /** Hello Contours **/

//    // Canny Edge Detector.
//    cv::Canny( img_gry, img_bin, maxThrash, minThresh, apertureSize );

//    // Contour Points.
//    std::vector<std::vector<cv::Point>> contours;

//    // Find Contours
//    cv::findContours( img_bin, contours, cv::noArray(), cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE );

//    // Reset Image data for Contours.
//    img_bin = cv::Scalar::all(0);

//    // Draw Contours
//    cv::drawContours( img_bin, contours, -1, cv::Scalar::all(255) );

//    // Display result-Image
//    cv::imshow( "Contours", img_bin );
}

/**
  * @brief
  * Search rectangular shape (quad localization using Hough algo.).
  *
  * (?)This methods uses canny + findContours + convexHull + approxPolyDP + drawContours.
  *
  * @param inGrey - Greyscale Image Matrix to work with.
  * @return - Rectangular Shape Points (4).
**/
std::vector<cv::Point> getQuad( cv::Mat& inGrey )
{
    // Result Output
    std::vector<cv::Point> result;

    // Polygon-Mask.
    cv::Mat approxPoly_mask(inGrey.rows, inGrey.cols, CV_8UC1);
    approxPoly_mask = cv::Scalar::all(0);

    // Contours
    std::vector<std::vector<cv::Point>> contours;
    findContours( inGrey, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE );

    // Indices
    std::vector<int> indices(contours.size());
    std::iota( indices.begin(), indices.end(), 0 );

    // Sort Contours using Points number, by desc.
    sort(indices.begin(), indices.end(), [&contours](int lhs, int rhs) { return contours[lhs].size() > contours[rhs].size(); });

    // Find the convex hull object for each contour
    std::vector<std::vector<cv::Point>> hull(1); // Allocate 1 Point.
    cv::convexHull( cv::Mat( contours[indices[0]] ), hull[0], false );

    // Search Polygons.
    std::vector<std::vector<cv::Point>> polygon(1);
    cv::approxPolyDP(hull[0], polygon[0], 20, true);

    // Draw only one Polygon.
    cv::drawContours(approxPoly_mask, polygon, 0, cv::Scalar(255));
    imshow("Poly-Mask", approxPoly_mask);

    // Return Rectangular Shape.
    if ( polygon[0].size() >= 4 )
    {
        return polygon[0];
    }

    return result;
}

int main( int pArgc, char* pArgv[] )
{

    /** Fast Connected Components Analyses **/

    /** Canny + Retinex **/

    // Create Windows
    cv::namedWindow( "Source", cv::WINDOW_FREERATIO );
    cv::resizeWindow( "Source", 320, 240 );
    cv::moveWindow( "Source", 0, 0 );
    cv::namedWindow( "Canny", cv::WINDOW_FREERATIO );
    cv::resizeWindow( "Canny", 320, 240 );
    cv::moveWindow( "Canny", 321, 0 );
    cv::namedWindow( "Poly-Mask", cv::WINDOW_FREERATIO );
    cv::resizeWindow( "Poly-Mask", 320, 240 );
    cv::moveWindow( "Poly-Mask", 642, 0 );
    cv::namedWindow( "Vertices", cv::WINDOW_FREERATIO );
    cv::resizeWindow( "Vertices", 320, 240 );
    cv::moveWindow( "Vertices", 0, 241 );
    cv::namedWindow( "Result", cv::WINDOW_FREERATIO );
    cv::resizeWindow( "Result", 320, 240 );
    cv::moveWindow( "Result", 321, 241 );

    // Greyscale Image.
    cv::Mat img_gry = cv::imread( "testimage01.jpg", cv::IMREAD_GRAYSCALE );
    cv::imshow( "Source", img_gry );

    // Blur
    cv::GaussianBlur( img_gry, img_gry, cv::Size(5, 5), 0 );

    // Canny
    cv::Mat img_cny;
    cv::Canny( img_gry, img_cny, 30, 100, 3 );
    cv::imshow( "Canny", img_cny ); // Show Canny.

    // Search Quad.
    std::vector<cv::Point> card_corners = getQuad( img_cny );

    // Create Image-Matrix to store interpolated-quad.
    cv::Mat warped_object( img_gry.rows, img_gry.cols, CV_8UC3 );

    // Homography only for rectangular shape.
    if ( card_corners.size() >= 4 )
    {
        // Describe vertices.
        cv::Mat homography = findHomography(card_corners, std::vector<cv::Point>{cv::Point(warped_object.cols, warped_object.rows), cv::Point(0, warped_object.rows), cv::Point(0, 0), cv::Point(warped_object.cols, 0)});

        // Perspective transform.
        cv::warpPerspective( img_gry, warped_object, homography, cv::Size(warped_object.cols, warped_object.rows) );
    }

    // Show Perspective object result.
    cv::imshow( "Result", warped_object );

    // Pause
    cv::waitKey();

    // Release Windows
    cv::destroyWindow( "Source" );
    cv::destroyWindow( "Canny" );
    cv::destroyWindow( "Poly-Mask" );
    cv::destroyWindow( "Vertices" );
    cv::destroyWindow( "Result" );

    /** Hello Canny **/
//    // Console-Output
//    std::cout << "Greyscale test . . ." << std::endl;

//    // Matrices
//    cv::Mat img_rgb, img_gry, img_cny;

//    // Create Windows
//    cv::namedWindow( "Gray Example" );
//    cv::namedWindow( "Canny Example" );

//    // Read RGB Image
//    img_rgb = cv::imread( "Neko-Maid.jpg", cv::IMREAD_COLOR );

//    // Converty RGB to Grey
//    cv::cvtColor( img_rgb, img_gry, cv::COLOR_BGR2GRAY );
//    cv::imshow( "Gray Example", img_gry ); // Display Grey Image

//    // Convert Grey to Canny (Canny Edge Detector, requires only 1-channel)
//    cv::Canny( img_gry, img_cny, 10, 100, 3 );
//    cv::imshow( "Canny Example", img_cny ); // Display Canny

//    // Wait Input
//    cv::waitKey();

//    // Release Windows
//    cv::destroyWindow( "Gray Example" );
//    cv::destroyWindow( "Canny Example" );

    /** Hello Threshold **/

//    // Create Windows
//    cv::namedWindow( "Threshold", cv::WINDOW_KEEPRATIO );
//    cv::resizeWindow( "Threshold", 320, 240 );

//    // Greyscale Image
//    cv::Mat img_gry = cv::imread( "Neko-Maid.jpg", cv::IMREAD_GRAYSCALE );
//    //cv::imshow( "Source - Grayscale", img_gry ); // Display Grey Image

//    // Blur
//    cv::Mat img_blurred;
//    cv::GaussianBlur(img_gry, img_blurred, cv::Size(5, 5), 0 );

//    // Threshold
//    cv::Mat img_thresh;
//    cv::threshold( img_blurred, img_thresh, 150, 254, cv::THRESH_BINARY );
//    cv::imshow( "Threshold", img_thresh ); // Display Threshold Image

//    // Pause
//    cv::waitKey();

//    // Release Windows
//    cv::destroyWindow( "Threshold" );

    /** Hello World ! **/
//	// Console-Output
//	std::cout << "Hello World !" << std::endl;
	
//	// Wait Input
//	std::cin.get();

//    // Create Material from Image-File.
//    cv::Mat img = cv::imread( "Neko-Maid.jpg", 1 );

//    //
//    cv::namedWindow( "Image Test" );

//    //
//    cv::imshow( "Image Test", img );

//    //
//    cv::waitKey();

//    //
//    cv::destroyWindow( "Image Test" );

	// OK
	return itdcv_OK;
}

// -----------------------------------------------------------
