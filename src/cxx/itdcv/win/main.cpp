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
// METHODS
// ===========================================================

int main( int pArgc, char* pArgv[] )
{
	
    // Console-Output
    std::cout << "Greyscale test . . ." << std::endl;

    // Matrices
    cv::Mat img_rgb, img_gry, img_cny;

    // Create Windows
    cv::namedWindow( "Gray Example" );
    cv::namedWindow( "Canny Example" );

    // Read RGB Image
    img_rgb = cv::imread( "Neko-Maid.jpg", cv::IMREAD_COLOR );

    // Converty RGB to Grey
    cv::cvtColor( img_rgb, img_gry, cv::COLOR_BGR2GRAY );
    cv::imshow( "Gray Example", img_gry ); // Display Grey Image

    // Convert Grey to Canny (Canny Edge Detector, requires only 1-channel)
    cv::Canny( img_gry, img_cny, 10, 100, 3 );
    cv::imshow( "Canny Example", img_cny ); // Display Canny

    // Wait Input
    cv::waitKey();

    // Release Windows
    cv::destroyWindow( "Gray Example" );
    cv::destroyWindow( "Canny Example" );

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
