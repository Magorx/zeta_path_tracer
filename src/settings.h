#include <PathTracer.hpp>


// settings ===================================================================

const int VERBOSITY = 2; // 2 for detailed log of some things

const int WINDOW_WIDTH  = 1000;
const int WINDOW_HEIGHT = 1000;

const int PIXEL_SCREEN_WIDTH  = 200;
// const int PIXEL_SCREEN_HEIGHT = 1000;

const int      PIXEL_SAMPLING   = 10;
const int      MAX_TRACE_DEPTH  = 10;

const double GAMMA_CORRECTION = 0.36;

const int DEFAULT_THREADS_CNT = 4;

// ============================================================================

const FramePostproc ACCUMULATOR_FRAME_POSTPROC = FramePostproc::copy;
const FramePostproc RENDERED_FRAME_POSTPROC    = FramePostproc::copy;

const double ACCUMULATOR_FRAME_POSTPROC_RADIUS = 1;
const double RENDERED_FRAME_POSTPROC_RADIUS = 1;

// ============================================================================

const int REAL_SCREEN_WIDTH   = 200;
const int REAL_SCREEN_HEIGHT  = 200;

const Vec3d  CAMERA_POS       = {-50, 50, 50};
const double CAMERA_VERTICAL_ROT   = 0;
const double CAMERA_HORIZONTAL_ROT = 0;
const double CAMERA_DIST       = 100;

const double RESOLUTION_COEF  =  (double) PIXEL_SCREEN_WIDTH / (double)  REAL_SCREEN_WIDTH;

const Vec3d  BACKGROUND_COLOR = {0, 0, 0};

// ============================================================================
