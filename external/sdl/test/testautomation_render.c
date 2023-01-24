/**
 * Original code: automated SDL platform test written by Edgar Simo "bobbens"
 * Extended and extensively updated by aschiffler at ferzkopp dot net
 */
#include <SDL3/SDL.h>
#include <SDL3/SDL_test.h>

/* ================= Test Case Implementation ================== */

#define TESTRENDER_SCREEN_W 80
#define TESTRENDER_SCREEN_H 60


#define RENDER_COMPARE_FORMAT SDL_PIXELFORMAT_ARGB8888

#define ALLOWABLE_ERROR_OPAQUE  0
#define ALLOWABLE_ERROR_BLENDED 64

/* Test window and renderer */
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

/* Prototypes for helper functions */

static int clearScreen(void);
static void compare(SDL_Surface *reference, int allowable_error);
static int hasTexAlpha(void);
static int hasTexColor(void);
static SDL_Texture *loadTestFace(void);
static int hasBlendModes(void);
static int hasDrawColor(void);
static int isSupported(int code);

/**
 * Create software renderer for tests
 */
void InitCreateRenderer(void *arg)
{
    int posX = 100, posY = 100, width = 320, height = 240;
    renderer = NULL;
    window = SDL_CreateWindow("render_testCreateRenderer", posX, posY, width, height, 0);
    SDLTest_AssertPass("SDL_CreateWindow()");
    SDLTest_AssertCheck(window != NULL, "Check SDL_CreateWindow result");
    if (window == NULL) {
        return;
    }

    renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED);
    SDLTest_AssertPass("SDL_CreateRenderer()");
    SDLTest_AssertCheck(renderer != NULL, "Check SDL_CreateRenderer result");
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        return;
    }
}

/*
 * Destroy renderer for tests
 */
void CleanupDestroyRenderer(void *arg)
{
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
        SDLTest_AssertPass("SDL_DestroyRenderer()");
    }

    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
        SDLTest_AssertPass("SDL_DestroyWindow");
    }
}

/**
 * @brief Tests call to SDL_GetNumRenderDrivers
 *
 * \sa
 * http://wiki.libsdl.org/SDL_GetNumRenderDrivers
 */
int render_testGetNumRenderDrivers(void *arg)
{
    int n;
    n = SDL_GetNumRenderDrivers();
    SDLTest_AssertCheck(n >= 1, "Number of renderers >= 1, reported as %i", n);
    return TEST_COMPLETED;
}

/**
 * @brief Tests the SDL primitives for rendering.
 *
 * \sa
 * http://wiki.libsdl.org/SDL_SetRenderDrawColor
 * http://wiki.libsdl.org/SDL_RenderFillRect
 * http://wiki.libsdl.org/SDL_RenderLine
 *
 */
int render_testPrimitives(void *arg)
{
    int ret;
    int x, y;
    SDL_FRect rect;
    SDL_Surface *referenceSurface = NULL;
    int checkFailCount1;
    int checkFailCount2;

    /* Clear surface. */
    clearScreen();

    /* Need drawcolor or just skip test. */
    SDLTest_AssertCheck(hasDrawColor(), "_hasDrawColor");

    /* Draw a rectangle. */
    rect.x = 40.0f;
    rect.y = 0.0f;
    rect.w = 40.0f;
    rect.h = 80.0f;

    ret = SDL_SetRenderDrawColor(renderer, 13, 73, 200, SDL_ALPHA_OPAQUE);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawColor, expected: 0, got: %i", ret);

    ret = SDL_RenderFillRect(renderer, &rect);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_RenderFillRect, expected: 0, got: %i", ret);

    /* Draw a rectangle. */
    rect.x = 10.0f;
    rect.y = 10.0f;
    rect.w = 60.0f;
    rect.h = 40.0f;
    ret = SDL_SetRenderDrawColor(renderer, 200, 0, 100, SDL_ALPHA_OPAQUE);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawColor, expected: 0, got: %i", ret);

    ret = SDL_RenderFillRect(renderer, &rect);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_RenderFillRect, expected: 0, got: %i", ret);

    /* Draw some points like so:
     * X.X.X.X..
     * .X.X.X.X.
     * X.X.X.X.. */
    checkFailCount1 = 0;
    checkFailCount2 = 0;
    for (y = 0; y < 3; y++) {
        for (x = y % 2; x < TESTRENDER_SCREEN_W; x += 2) {
            ret = SDL_SetRenderDrawColor(renderer, x * y, x * y / 2, x * y / 3, SDL_ALPHA_OPAQUE);
            if (ret != 0) {
                checkFailCount1++;
            }

            ret = SDL_RenderPoint(renderer, (float)x, (float)y);
            if (ret != 0) {
                checkFailCount2++;
            }
        }
    }
    SDLTest_AssertCheck(checkFailCount1 == 0, "Validate results from calls to SDL_SetRenderDrawColor, expected: 0, got: %i", checkFailCount1);
    SDLTest_AssertCheck(checkFailCount2 == 0, "Validate results from calls to SDL_RenderPoint, expected: 0, got: %i", checkFailCount2);

    /* Draw some lines. */
    ret = SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawColor");

    ret = SDL_RenderLine(renderer, 0.0f, 30.0f, (float)TESTRENDER_SCREEN_W, 30.0f);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_RenderLine, expected: 0, got: %i", ret);

    ret = SDL_SetRenderDrawColor(renderer, 55, 55, 5, SDL_ALPHA_OPAQUE);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawColor, expected: 0, got: %i", ret);

    ret = SDL_RenderLine(renderer, 40.0f, 30.0f, 40.0f, 60.0f);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_RenderLine, expected: 0, got: %i", ret);

    ret = SDL_SetRenderDrawColor(renderer, 5, 105, 105, SDL_ALPHA_OPAQUE);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawColor, expected: 0, got: %i", ret);

    ret = SDL_RenderLine(renderer, 0.0f, 0.0f, 29.0f, 29.0f);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_RenderLine, expected: 0, got: %i", ret);

    ret = SDL_RenderLine(renderer, 29.0f, 30.0f, 0.0f, 59.0f);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_RenderLine, expected: 0, got: %i", ret);

    ret = SDL_RenderLine(renderer, 79.0f, 0.0f, 50.0f, 29.0f);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_RenderLine, expected: 0, got: %i", ret);

    ret = SDL_RenderLine(renderer, 79.0f, 59.0f, 50.0f, 30.0f);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_RenderLine, expected: 0, got: %i", ret);

    /* See if it's the same. */
    referenceSurface = SDLTest_ImagePrimitives();
    compare(referenceSurface, ALLOWABLE_ERROR_OPAQUE);

    /* Make current */
    SDL_RenderPresent(renderer);

    /* Clean up. */
    SDL_DestroySurface(referenceSurface);
    referenceSurface = NULL;

    return TEST_COMPLETED;
}

/**
 * @brief Tests the SDL primitives with alpha for rendering.
 *
 * \sa
 * http://wiki.libsdl.org/SDL_SetRenderDrawColor
 * http://wiki.libsdl.org/SDL_SetRenderDrawBlendMode
 * http://wiki.libsdl.org/SDL_RenderFillRect
 */
int render_testPrimitivesBlend(void *arg)
{
    int ret;
    int i, j;
    SDL_FRect rect;
    SDL_Surface *referenceSurface = NULL;
    int checkFailCount1;
    int checkFailCount2;
    int checkFailCount3;

    /* Clear surface. */
    clearScreen();

    /* Need drawcolor and blendmode or just skip test. */
    SDLTest_AssertCheck(hasDrawColor(), "_hasDrawColor");
    SDLTest_AssertCheck(hasBlendModes(), "_hasBlendModes");

    /* Create some rectangles for each blend mode. */
    ret = SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawColor, expected: 0, got: %i", ret);

    ret = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawBlendMode, expected: 0, got: %i", ret);

    ret = SDL_RenderFillRect(renderer, NULL);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_RenderFillRect, expected: 0, got: %i", ret);

    rect.x = 10.0f;
    rect.y = 25.0f;
    rect.w = 40.0f;
    rect.h = 25.0f;
    ret = SDL_SetRenderDrawColor(renderer, 240, 10, 10, 75);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawColor, expected: 0, got: %i", ret);

    ret = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawBlendMode, expected: 0, got: %i", ret);

    ret = SDL_RenderFillRect(renderer, &rect);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_RenderFillRect, expected: 0, got: %i", ret);

    rect.x = 30.0f;
    rect.y = 40.0f;
    rect.w = 45.0f;
    rect.h = 15.0f;
    ret = SDL_SetRenderDrawColor(renderer, 10, 240, 10, 100);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawColor, expected: 0, got: %i", ret);

    ret = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawBlendMode, expected: 0, got: %i", ret);

    ret = SDL_RenderFillRect(renderer, &rect);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_RenderFillRect, expected: 0, got: %i", ret);

    rect.x = 25.0f;
    rect.y = 25.0f;
    rect.w = 25.0f;
    rect.h = 25.0f;
    ret = SDL_SetRenderDrawColor(renderer, 10, 10, 240, 125);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawColor, expected: 0, got: %i", ret);

    ret = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawBlendMode, expected: 0, got: %i", ret);

    ret = SDL_RenderFillRect(renderer, &rect);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_RenderFillRect, expected: 0, got: %i", ret);

    /* Draw blended lines, lines for everyone. */
    checkFailCount1 = 0;
    checkFailCount2 = 0;
    checkFailCount3 = 0;
    for (i = 0; i < TESTRENDER_SCREEN_W; i += 2) {
        ret = SDL_SetRenderDrawColor(renderer, 60 + 2 * i, 240 - 2 * i, 50, 3 * i);
        if (ret != 0) {
            checkFailCount1++;
        }

        ret = SDL_SetRenderDrawBlendMode(renderer, (((i / 2) % 3) == 0) ? SDL_BLENDMODE_BLEND : (((i / 2) % 3) == 1) ? SDL_BLENDMODE_ADD
                                                                                                                     : SDL_BLENDMODE_NONE);
        if (ret != 0) {
            checkFailCount2++;
        }

        ret = SDL_RenderLine(renderer, 0.0f, 0.0f, (float)i, 59.0f);
        if (ret != 0) {
            checkFailCount3++;
        }
    }
    SDLTest_AssertCheck(checkFailCount1 == 0, "Validate results from calls to SDL_SetRenderDrawColor, expected: 0, got: %i", checkFailCount1);
    SDLTest_AssertCheck(checkFailCount2 == 0, "Validate results from calls to SDL_SetRenderDrawBlendMode, expected: 0, got: %i", checkFailCount2);
    SDLTest_AssertCheck(checkFailCount3 == 0, "Validate results from calls to SDL_RenderLine, expected: 0, got: %i", checkFailCount3);

    checkFailCount1 = 0;
    checkFailCount2 = 0;
    checkFailCount3 = 0;
    for (i = 0; i < TESTRENDER_SCREEN_H; i += 2) {
        ret = SDL_SetRenderDrawColor(renderer, 60 + 2 * i, 240 - 2 * i, 50, 3 * i);
        if (ret != 0) {
            checkFailCount1++;
        }

        ret = SDL_SetRenderDrawBlendMode(renderer, (((i / 2) % 3) == 0) ? SDL_BLENDMODE_BLEND : (((i / 2) % 3) == 1) ? SDL_BLENDMODE_ADD
                                                                                                                     : SDL_BLENDMODE_NONE);
        if (ret != 0) {
            checkFailCount2++;
        }

        ret = SDL_RenderLine(renderer, 0.0f, 0.0f, 79.0f, (float)i);
        if (ret != 0) {
            checkFailCount3++;
        }
    }
    SDLTest_AssertCheck(checkFailCount1 == 0, "Validate results from calls to SDL_SetRenderDrawColor, expected: 0, got: %i", checkFailCount1);
    SDLTest_AssertCheck(checkFailCount2 == 0, "Validate results from calls to SDL_SetRenderDrawBlendMode, expected: 0, got: %i", checkFailCount2);
    SDLTest_AssertCheck(checkFailCount3 == 0, "Validate results from calls to SDL_RenderLine, expected: 0, got: %i", checkFailCount3);

    /* Draw points. */
    checkFailCount1 = 0;
    checkFailCount2 = 0;
    checkFailCount3 = 0;
    for (j = 0; j < TESTRENDER_SCREEN_H; j += 3) {
        for (i = 0; i < TESTRENDER_SCREEN_W; i += 3) {
            ret = SDL_SetRenderDrawColor(renderer, j * 4, i * 3, j * 4, i * 3);
            if (ret != 0) {
                checkFailCount1++;
            }

            ret = SDL_SetRenderDrawBlendMode(renderer, ((((i + j) / 3) % 3) == 0) ? SDL_BLENDMODE_BLEND : ((((i + j) / 3) % 3) == 1) ? SDL_BLENDMODE_ADD
                                                                                                                                     : SDL_BLENDMODE_NONE);
            if (ret != 0) {
                checkFailCount2++;
            }

            ret = SDL_RenderPoint(renderer, (float)i, (float)j);
            if (ret != 0) {
                checkFailCount3++;
            }
        }
    }
    SDLTest_AssertCheck(checkFailCount1 == 0, "Validate results from calls to SDL_SetRenderDrawColor, expected: 0, got: %i", checkFailCount1);
    SDLTest_AssertCheck(checkFailCount2 == 0, "Validate results from calls to SDL_SetRenderDrawBlendMode, expected: 0, got: %i", checkFailCount2);
    SDLTest_AssertCheck(checkFailCount3 == 0, "Validate results from calls to SDL_RenderPoint, expected: 0, got: %i", checkFailCount3);

    /* See if it's the same. */
    referenceSurface = SDLTest_ImagePrimitivesBlend();
    compare(referenceSurface, ALLOWABLE_ERROR_BLENDED);

    /* Make current */
    SDL_RenderPresent(renderer);

    /* Clean up. */
    SDL_DestroySurface(referenceSurface);
    referenceSurface = NULL;

    return TEST_COMPLETED;
}

/**
 * @brief Tests some blitting routines.
 *
 * \sa
 * http://wiki.libsdl.org/SDL_RenderTexture
 * http://wiki.libsdl.org/SDL_DestroyTexture
 */
int render_testBlit(void *arg)
{
    int ret;
    SDL_FRect rect;
    SDL_Texture *tface;
    SDL_Surface *referenceSurface = NULL;
    Uint32 tformat;
    int taccess, tw, th;
    int i, j, ni, nj;
    int checkFailCount1;

    /* Clear surface. */
    clearScreen();

    /* Need drawcolor or just skip test. */
    SDLTest_AssertCheck(hasDrawColor(), "_hasDrawColor)");

    /* Create face surface. */
    tface = loadTestFace();
    SDLTest_AssertCheck(tface != NULL, "Verify loadTestFace() result");
    if (tface == NULL) {
        return TEST_ABORTED;
    }

    /* Constant values. */
    ret = SDL_QueryTexture(tface, &tformat, &taccess, &tw, &th);
    SDLTest_AssertCheck(ret == 0, "Verify result from SDL_QueryTexture, expected 0, got %i", ret);
    rect.w = (float)tw;
    rect.h = (float)th;
    ni = TESTRENDER_SCREEN_W - tw;
    nj = TESTRENDER_SCREEN_H - th;

    /* Loop blit. */
    checkFailCount1 = 0;
    for (j = 0; j <= nj; j += 4) {
        for (i = 0; i <= ni; i += 4) {
            /* Blitting. */
            rect.x = (float)i;
            rect.y = (float)j;
            ret = SDL_RenderTexture(renderer, tface, NULL, &rect);
            if (ret != 0) {
                checkFailCount1++;
            }
        }
    }
    SDLTest_AssertCheck(checkFailCount1 == 0, "Validate results from calls to SDL_RenderTexture, expected: 0, got: %i", checkFailCount1);

    /* See if it's the same */
    referenceSurface = SDLTest_ImageBlit();
    compare(referenceSurface, ALLOWABLE_ERROR_OPAQUE);

    /* Make current */
    SDL_RenderPresent(renderer);

    /* Clean up. */
    SDL_DestroyTexture(tface);
    SDL_DestroySurface(referenceSurface);
    referenceSurface = NULL;

    return TEST_COMPLETED;
}

/**
 * @brief Blits doing color tests.
 *
 * \sa
 * http://wiki.libsdl.org/SDL_SetTextureColorMod
 * http://wiki.libsdl.org/SDL_RenderTexture
 * http://wiki.libsdl.org/SDL_DestroyTexture
 */
int render_testBlitColor(void *arg)
{
    int ret;
    SDL_FRect rect;
    SDL_Texture *tface;
    SDL_Surface *referenceSurface = NULL;
    Uint32 tformat;
    int taccess, tw, th;
    int i, j, ni, nj;
    int checkFailCount1;
    int checkFailCount2;

    /* Clear surface. */
    clearScreen();

    /* Create face surface. */
    tface = loadTestFace();
    SDLTest_AssertCheck(tface != NULL, "Verify loadTestFace() result");
    if (tface == NULL) {
        return TEST_ABORTED;
    }

    /* Constant values. */
    ret = SDL_QueryTexture(tface, &tformat, &taccess, &tw, &th);
    SDLTest_AssertCheck(ret == 0, "Verify result from SDL_QueryTexture, expected 0, got %i", ret);
    rect.w = (float)tw;
    rect.h = (float)th;
    ni = TESTRENDER_SCREEN_W - tw;
    nj = TESTRENDER_SCREEN_H - th;

    /* Test blitting with color mod. */
    checkFailCount1 = 0;
    checkFailCount2 = 0;
    for (j = 0; j <= nj; j += 4) {
        for (i = 0; i <= ni; i += 4) {
            /* Set color mod. */
            ret = SDL_SetTextureColorMod(tface, (255 / nj) * j, (255 / ni) * i, (255 / nj) * j);
            if (ret != 0) {
                checkFailCount1++;
            }

            /* Blitting. */
            rect.x = (float)i;
            rect.y = (float)j;
            ret = SDL_RenderTexture(renderer, tface, NULL, &rect);
            if (ret != 0) {
                checkFailCount2++;
            }
        }
    }
    SDLTest_AssertCheck(checkFailCount1 == 0, "Validate results from calls to SDL_SetTextureColorMod, expected: 0, got: %i", checkFailCount1);
    SDLTest_AssertCheck(checkFailCount2 == 0, "Validate results from calls to SDL_RenderTexture, expected: 0, got: %i", checkFailCount2);

    /* See if it's the same. */
    referenceSurface = SDLTest_ImageBlitColor();
    compare(referenceSurface, ALLOWABLE_ERROR_OPAQUE);

    /* Make current */
    SDL_RenderPresent(renderer);

    /* Clean up. */
    SDL_DestroyTexture(tface);
    SDL_DestroySurface(referenceSurface);
    referenceSurface = NULL;

    return TEST_COMPLETED;
}

/**
 * @brief Tests blitting with alpha.
 *
 * \sa
 * http://wiki.libsdl.org/SDL_SetTextureAlphaMod
 * http://wiki.libsdl.org/SDL_RenderTexture
 * http://wiki.libsdl.org/SDL_DestroyTexture
 */
int render_testBlitAlpha(void *arg)
{
    int ret;
    SDL_FRect rect;
    SDL_Texture *tface;
    SDL_Surface *referenceSurface = NULL;
    Uint32 tformat;
    int taccess, tw, th;
    int i, j, ni, nj;
    int checkFailCount1;
    int checkFailCount2;

    /* Clear surface. */
    clearScreen();

    /* Need alpha or just skip test. */
    SDLTest_AssertCheck(hasTexAlpha(), "_hasTexAlpha");

    /* Create face surface. */
    tface = loadTestFace();
    SDLTest_AssertCheck(tface != NULL, "Verify loadTestFace() result");
    if (tface == NULL) {
        return TEST_ABORTED;
    }

    /* Constant values. */
    ret = SDL_QueryTexture(tface, &tformat, &taccess, &tw, &th);
    SDLTest_AssertCheck(ret == 0, "Verify result from SDL_QueryTexture, expected 0, got %i", ret);
    rect.w = (float)tw;
    rect.h = (float)th;
    ni = TESTRENDER_SCREEN_W - tw;
    nj = TESTRENDER_SCREEN_H - th;

    /* Test blitting with alpha mod. */
    checkFailCount1 = 0;
    checkFailCount2 = 0;
    for (j = 0; j <= nj; j += 4) {
        for (i = 0; i <= ni; i += 4) {
            /* Set alpha mod. */
            ret = SDL_SetTextureAlphaMod(tface, (255 / ni) * i);
            if (ret != 0) {
                checkFailCount1++;
            }

            /* Blitting. */
            rect.x = (float)i;
            rect.y = (float)j;
            ret = SDL_RenderTexture(renderer, tface, NULL, &rect);
            if (ret != 0) {
                checkFailCount2++;
            }
        }
    }
    SDLTest_AssertCheck(checkFailCount1 == 0, "Validate results from calls to SDL_SetTextureAlphaMod, expected: 0, got: %i", checkFailCount1);
    SDLTest_AssertCheck(checkFailCount2 == 0, "Validate results from calls to SDL_RenderTexture, expected: 0, got: %i", checkFailCount2);

    /* See if it's the same. */
    referenceSurface = SDLTest_ImageBlitAlpha();
    compare(referenceSurface, ALLOWABLE_ERROR_BLENDED);

    /* Make current */
    SDL_RenderPresent(renderer);

    /* Clean up. */
    SDL_DestroyTexture(tface);
    SDL_DestroySurface(referenceSurface);
    referenceSurface = NULL;

    return TEST_COMPLETED;
}

/* Helper functions */

/**
 * @brief Tests a blend mode.
 *
 * \sa
 * http://wiki.libsdl.org/SDL_SetTextureBlendMode
 * http://wiki.libsdl.org/SDL_RenderTexture
 */
static void
testBlitBlendMode(SDL_Texture *tface, int mode)
{
    int ret;
    Uint32 tformat;
    int taccess, tw, th;
    int i, j, ni, nj;
    SDL_FRect rect;
    int checkFailCount1;
    int checkFailCount2;

    /* Clear surface. */
    clearScreen();

    /* Constant values. */
    ret = SDL_QueryTexture(tface, &tformat, &taccess, &tw, &th);
    SDLTest_AssertCheck(ret == 0, "Verify result from SDL_QueryTexture, expected 0, got %i", ret);
    rect.w = (float)tw;
    rect.h = (float)th;
    ni = TESTRENDER_SCREEN_W - tw;
    nj = TESTRENDER_SCREEN_H - th;

    /* Test blend mode. */
    checkFailCount1 = 0;
    checkFailCount2 = 0;
    for (j = 0; j <= nj; j += 4) {
        for (i = 0; i <= ni; i += 4) {
            /* Set blend mode. */
            ret = SDL_SetTextureBlendMode(tface, (SDL_BlendMode)mode);
            if (ret != 0) {
                checkFailCount1++;
            }

            /* Blitting. */
            rect.x = (float)i;
            rect.y = (float)j;
            ret = SDL_RenderTexture(renderer, tface, NULL, &rect);
            if (ret != 0) {
                checkFailCount2++;
            }
        }
    }
    SDLTest_AssertCheck(checkFailCount1 == 0, "Validate results from calls to SDL_SetTextureBlendMode, expected: 0, got: %i", checkFailCount1);
    SDLTest_AssertCheck(checkFailCount2 == 0, "Validate results from calls to SDL_RenderTexture, expected: 0, got: %i", checkFailCount2);
}

/**
 * @brief Tests some more blitting routines.
 *
 * \sa
 * http://wiki.libsdl.org/SDL_SetTextureColorMod
 * http://wiki.libsdl.org/SDL_SetTextureAlphaMod
 * http://wiki.libsdl.org/SDL_SetTextureBlendMode
 * http://wiki.libsdl.org/SDL_DestroyTexture
 */
int render_testBlitBlend(void *arg)
{
    int ret;
    SDL_FRect rect;
    SDL_Texture *tface;
    SDL_Surface *referenceSurface = NULL;
    Uint32 tformat;
    int taccess, tw, th;
    int i, j, ni, nj;
    int mode;
    int checkFailCount1;
    int checkFailCount2;
    int checkFailCount3;
    int checkFailCount4;

    SDLTest_AssertCheck(hasBlendModes(), "_hasBlendModes");
    SDLTest_AssertCheck(hasTexColor(), "_hasTexColor");
    SDLTest_AssertCheck(hasTexAlpha(), "_hasTexAlpha");

    /* Create face surface. */
    tface = loadTestFace();
    SDLTest_AssertCheck(tface != NULL, "Verify loadTestFace() result");
    if (tface == NULL) {
        return TEST_ABORTED;
    }

    /* Constant values. */
    ret = SDL_QueryTexture(tface, &tformat, &taccess, &tw, &th);
    SDLTest_AssertCheck(ret == 0, "Verify result from SDL_QueryTexture, expected 0, got %i", ret);
    rect.w = (float)tw;
    rect.h = (float)th;
    ni = TESTRENDER_SCREEN_W - tw;
    nj = TESTRENDER_SCREEN_H - th;

    /* Set alpha mod. */
    ret = SDL_SetTextureAlphaMod(tface, 100);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetTextureAlphaMod, expected: 0, got: %i", ret);

    /* Test None. */
    testBlitBlendMode(tface, SDL_BLENDMODE_NONE);
    referenceSurface = SDLTest_ImageBlitBlendNone();

    /* Compare, then Present */
    compare(referenceSurface, ALLOWABLE_ERROR_OPAQUE);
    SDL_RenderPresent(renderer);

    SDL_DestroySurface(referenceSurface);
    referenceSurface = NULL;

    /* Test Blend. */
    testBlitBlendMode(tface, SDL_BLENDMODE_BLEND);
    referenceSurface = SDLTest_ImageBlitBlend();

    /* Compare, then Present */
    compare(referenceSurface, ALLOWABLE_ERROR_BLENDED);
    SDL_RenderPresent(renderer);

    SDL_DestroySurface(referenceSurface);
    referenceSurface = NULL;

    /* Test Add. */
    testBlitBlendMode(tface, SDL_BLENDMODE_ADD);
    referenceSurface = SDLTest_ImageBlitBlendAdd();

    /* Compare, then Present */
    compare(referenceSurface, ALLOWABLE_ERROR_BLENDED);
    SDL_RenderPresent(renderer);

    SDL_DestroySurface(referenceSurface);
    referenceSurface = NULL;

    /* Test Mod. */
    testBlitBlendMode(tface, SDL_BLENDMODE_MOD);
    referenceSurface = SDLTest_ImageBlitBlendMod();

    /* Compare, then Present */
    compare(referenceSurface, ALLOWABLE_ERROR_BLENDED);
    SDL_RenderPresent(renderer);

    SDL_DestroySurface(referenceSurface);
    referenceSurface = NULL;

    /* Clear surface. */
    clearScreen();

    /* Loop blit. */
    checkFailCount1 = 0;
    checkFailCount2 = 0;
    checkFailCount3 = 0;
    checkFailCount4 = 0;
    for (j = 0; j <= nj; j += 4) {
        for (i = 0; i <= ni; i += 4) {

            /* Set color mod. */
            ret = SDL_SetTextureColorMod(tface, (255 / nj) * j, (255 / ni) * i, (255 / nj) * j);
            if (ret != 0) {
                checkFailCount1++;
            }

            /* Set alpha mod. */
            ret = SDL_SetTextureAlphaMod(tface, (100 / ni) * i);
            if (ret != 0) {
                checkFailCount2++;
            }

            /* Crazy blending mode magic. */
            mode = (i / 4 * j / 4) % 4;
            if (mode == 0) {
                mode = SDL_BLENDMODE_NONE;
            } else if (mode == 1) {
                mode = SDL_BLENDMODE_BLEND;
            } else if (mode == 2) {
                mode = SDL_BLENDMODE_ADD;
            } else if (mode == 3) {
                mode = SDL_BLENDMODE_MOD;
            }
            ret = SDL_SetTextureBlendMode(tface, (SDL_BlendMode)mode);
            if (ret != 0) {
                checkFailCount3++;
            }

            /* Blitting. */
            rect.x = (float)i;
            rect.y = (float)j;
            ret = SDL_RenderTexture(renderer, tface, NULL, &rect);
            if (ret != 0) {
                checkFailCount4++;
            }
        }
    }
    SDLTest_AssertCheck(checkFailCount1 == 0, "Validate results from calls to SDL_SetTextureColorMod, expected: 0, got: %i", checkFailCount1);
    SDLTest_AssertCheck(checkFailCount2 == 0, "Validate results from calls to SDL_SetTextureAlphaMod, expected: 0, got: %i", checkFailCount2);
    SDLTest_AssertCheck(checkFailCount3 == 0, "Validate results from calls to SDL_SetTextureBlendMode, expected: 0, got: %i", checkFailCount3);
    SDLTest_AssertCheck(checkFailCount4 == 0, "Validate results from calls to SDL_RenderTexture, expected: 0, got: %i", checkFailCount4);

    /* Clean up. */
    SDL_DestroyTexture(tface);

    /* Check to see if final image matches. */
    referenceSurface = SDLTest_ImageBlitBlendAll();
    compare(referenceSurface, ALLOWABLE_ERROR_BLENDED);

    /* Make current */
    SDL_RenderPresent(renderer);

    SDL_DestroySurface(referenceSurface);
    referenceSurface = NULL;

    return TEST_COMPLETED;
}

/**
 * @brief Checks to see if functionality is supported. Helper function.
 */
static int
isSupported(int code)
{
    return code == 0;
}

/**
 * @brief Test to see if we can vary the draw color. Helper function.
 *
 * \sa
 * http://wiki.libsdl.org/SDL_SetRenderDrawColor
 * http://wiki.libsdl.org/SDL_GetRenderDrawColor
 */
static int
hasDrawColor(void)
{
    int ret, fail;
    Uint8 r, g, b, a;

    fail = 0;

    /* Set color. */
    ret = SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    if (!isSupported(ret)) {
        fail = 1;
    }

    /* Restore natural. */
    ret = SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    if (!isSupported(ret)) {
        fail = 1;
    }

    /* Something failed, consider not available. */
    if (fail) {
        return 0;
    }
    /* Not set properly, consider failed. */
    else if ((r != 100) || (g != 100) || (b != 100) || (a != 100)) {
        return 0;
    }
    return 1;
}

/**
 * @brief Test to see if we can vary the blend mode. Helper function.
 *
 * \sa
 * http://wiki.libsdl.org/SDL_SetRenderDrawBlendMode
 * http://wiki.libsdl.org/SDL_GetRenderDrawBlendMode
 */
static int
hasBlendModes(void)
{
    int fail;
    int ret;
    SDL_BlendMode mode;

    fail = 0;

    ret = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = SDL_GetRenderDrawBlendMode(renderer, &mode);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = (mode != SDL_BLENDMODE_BLEND);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = SDL_GetRenderDrawBlendMode(renderer, &mode);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = (mode != SDL_BLENDMODE_ADD);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_MOD);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = SDL_GetRenderDrawBlendMode(renderer, &mode);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = (mode != SDL_BLENDMODE_MOD);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = SDL_GetRenderDrawBlendMode(renderer, &mode);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = (mode != SDL_BLENDMODE_NONE);
    if (!isSupported(ret)) {
        fail = 1;
    }

    return !fail;
}

/**
 * @brief Loads the test image 'Face' as texture. Helper function.
 *
 * \sa
 * http://wiki.libsdl.org/SDL_CreateTextureFromSurface
 */
static SDL_Texture *
loadTestFace(void)
{
    SDL_Surface *face;
    SDL_Texture *tface;

    face = SDLTest_ImageFace();
    if (face == NULL) {
        return NULL;
    }

    tface = SDL_CreateTextureFromSurface(renderer, face);
    if (tface == NULL) {
        SDLTest_LogError("SDL_CreateTextureFromSurface() failed with error: %s", SDL_GetError());
    }

    SDL_DestroySurface(face);

    return tface;
}

/**
 * @brief Test to see if can set texture color mode. Helper function.
 *
 * \sa
 * http://wiki.libsdl.org/SDL_SetTextureColorMod
 * http://wiki.libsdl.org/SDL_GetTextureColorMod
 * http://wiki.libsdl.org/SDL_DestroyTexture
 */
static int
hasTexColor(void)
{
    int fail;
    int ret;
    SDL_Texture *tface;
    Uint8 r, g, b;

    /* Get test face. */
    tface = loadTestFace();
    if (tface == NULL) {
        return 0;
    }

    /* See if supported. */
    fail = 0;
    ret = SDL_SetTextureColorMod(tface, 100, 100, 100);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = SDL_GetTextureColorMod(tface, &r, &g, &b);
    if (!isSupported(ret)) {
        fail = 1;
    }

    /* Clean up. */
    SDL_DestroyTexture(tface);

    if (fail) {
        return 0;
    } else if ((r != 100) || (g != 100) || (b != 100)) {
        return 0;
    }
    return 1;
}

/**
 * @brief Test to see if we can vary the alpha of the texture. Helper function.
 *
 * \sa
 *  http://wiki.libsdl.org/SDL_SetTextureAlphaMod
 *  http://wiki.libsdl.org/SDL_GetTextureAlphaMod
 *  http://wiki.libsdl.org/SDL_DestroyTexture
 */
static int
hasTexAlpha(void)
{
    int fail;
    int ret;
    SDL_Texture *tface;
    Uint8 a;

    /* Get test face. */
    tface = loadTestFace();
    if (tface == NULL) {
        return 0;
    }

    /* See if supported. */
    fail = 0;
    ret = SDL_SetTextureAlphaMod(tface, 100);
    if (!isSupported(ret)) {
        fail = 1;
    }
    ret = SDL_GetTextureAlphaMod(tface, &a);
    if (!isSupported(ret)) {
        fail = 1;
    }

    /* Clean up. */
    SDL_DestroyTexture(tface);

    if (fail) {
        return 0;
    } else if (a != 100) {
        return 0;
    }
    return 1;
}

/**
 * @brief Compares screen pixels with image pixels. Helper function.
 *
 * @param referenceSurface Image to compare against.
 * @param allowable_error allowed difference from the reference image
 *
 * \sa
 * http://wiki.libsdl.org/SDL_RenderReadPixels
 * http://wiki.libsdl.org/SDL_CreateSurfaceFrom
 * http://wiki.libsdl.org/SDL_DestroySurface
 */
static void
compare(SDL_Surface *referenceSurface, int allowable_error)
{
   int result;
   SDL_Rect rect;
   Uint8 *pixels;
   SDL_Surface *testSurface;

   /* Read pixels. */
   pixels = (Uint8 *)SDL_malloc(4*TESTRENDER_SCREEN_W*TESTRENDER_SCREEN_H);
   SDLTest_AssertCheck(pixels != NULL, "Validate allocated temp pixel buffer");
   if (pixels == NULL) {
      return;
   }

   /* Explicitly specify the rect in case the window isn't the expected size... */
   rect.x = 0;
   rect.y = 0;
   rect.w = TESTRENDER_SCREEN_W;
   rect.h = TESTRENDER_SCREEN_H;
   result = SDL_RenderReadPixels(renderer, &rect, RENDER_COMPARE_FORMAT, pixels, 80*4 );
   SDLTest_AssertCheck(result == 0, "Validate result from SDL_RenderReadPixels, expected: 0, got: %i", result);

   /* Create surface. */
   testSurface = SDL_CreateSurfaceFrom(pixels, TESTRENDER_SCREEN_W, TESTRENDER_SCREEN_H, TESTRENDER_SCREEN_W*4, RENDER_COMPARE_FORMAT);
   SDLTest_AssertCheck(testSurface != NULL, "Verify result from SDL_CreateSurfaceFrom is not NULL");

   /* Compare surface. */
   result = SDLTest_CompareSurfaces( testSurface, referenceSurface, allowable_error );
   SDLTest_AssertCheck(result == 0, "Validate result from SDLTest_CompareSurfaces, expected: 0, got: %i", result);

   /* Clean up. */
   SDL_free(pixels);
   SDL_DestroySurface(testSurface);
}

/**
 * @brief Clears the screen. Helper function.
 *
 * \sa
 * http://wiki.libsdl.org/SDL_SetRenderDrawColor
 * http://wiki.libsdl.org/SDL_RenderClear
 * http://wiki.libsdl.org/SDL_RenderPresent
 * http://wiki.libsdl.org/SDL_SetRenderDrawBlendMode
 */
static int
clearScreen(void)
{
    int ret;

    /* Set color. */
    ret = SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawColor, expected: 0, got: %i", ret);

    /* Clear screen. */
    ret = SDL_RenderClear(renderer);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_RenderClear, expected: 0, got: %i", ret);

    /* Make current */
    SDL_RenderPresent(renderer);

    /* Set defaults. */
    ret = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawBlendMode, expected: 0, got: %i", ret);

    ret = SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDLTest_AssertCheck(ret == 0, "Validate result from SDL_SetRenderDrawColor, expected: 0, got: %i", ret);

    return 0;
}

/* ================= Test References ================== */

/* Render test cases */
static const SDLTest_TestCaseReference renderTest1 = {
    (SDLTest_TestCaseFp)render_testGetNumRenderDrivers, "render_testGetNumRenderDrivers", "Tests call to SDL_GetNumRenderDrivers", TEST_ENABLED
};

static const SDLTest_TestCaseReference renderTest2 = {
    (SDLTest_TestCaseFp)render_testPrimitives, "render_testPrimitives", "Tests rendering primitives", TEST_ENABLED
};

/* TODO: rewrite test case, define new test data and re-enable; current implementation fails */
static const SDLTest_TestCaseReference renderTest3 = {
    (SDLTest_TestCaseFp)render_testPrimitivesBlend, "render_testPrimitivesBlend", "Tests rendering primitives with blending", TEST_DISABLED
};

static const SDLTest_TestCaseReference renderTest4 = {
    (SDLTest_TestCaseFp)render_testBlit, "render_testBlit", "Tests blitting", TEST_ENABLED
};

static const SDLTest_TestCaseReference renderTest5 = {
    (SDLTest_TestCaseFp)render_testBlitColor, "render_testBlitColor", "Tests blitting with color", TEST_ENABLED
};

/* TODO: rewrite test case, define new test data and re-enable; current implementation fails */
static const SDLTest_TestCaseReference renderTest6 = {
    (SDLTest_TestCaseFp)render_testBlitAlpha, "render_testBlitAlpha", "Tests blitting with alpha", TEST_DISABLED
};

/* TODO: rewrite test case, define new test data and re-enable; current implementation fails */
static const SDLTest_TestCaseReference renderTest7 = {
    (SDLTest_TestCaseFp)render_testBlitBlend, "render_testBlitBlend", "Tests blitting with blending", TEST_DISABLED
};

/* Sequence of Render test cases */
static const SDLTest_TestCaseReference *renderTests[] = {
    &renderTest1, &renderTest2, &renderTest3, &renderTest4, &renderTest5, &renderTest6, &renderTest7, NULL
};

/* Render test suite (global) */
SDLTest_TestSuiteReference renderTestSuite = {
    "Render",
    InitCreateRenderer,
    renderTests,
    CleanupDestroyRenderer
};