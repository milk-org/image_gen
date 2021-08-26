#include "CommandLineInterface/CLIcore.h"
#include "statistic/statistic.h"


// Local variables pointers
static LOCVAR_OUTIMG2D outim;
static int *distrib;

static CLICMDARGDEF farg[] =
{
    FARG_OUTIM2D(outim),
    {
        CLIARG_LONG, ".distrib",
        "distribution \n"
        " (0: uniform)\n"
        " (1: gauss)\n"
        " (2: truncated gauss)\n",
        "0",
        CLIARG_HIDDEN_DEFAULT,
        (void **) &distrib
    }
};

static CLICMDDATA CLIcmddata =
{
    "mkrnd",
    "make random image",
    CLICMD_FIELDS_DEFAULTS
};


// detailed help
static errno_t help_function()
{
    return RETURN_SUCCESS;
}



/**
 * @brief Make random image
 *
 *
 * @param[out] img
 *      Output image
 *
 * @param[in] pdf
 *      Probability distribution function
 *
 * @return imageID
 */
static imageID make_image_random(
    IMGID *img,
    int pdf
)
{
                   DEBUG_TRACE_FSTART();

    // 0: uniform
    // 1: gauss
    // 2: truncated gauss


    // Create image if needed
    imcreateIMGID(img);


    // openMP is slow when calling gsl random number generator : do not use openMP here
    if(pdf == 0)
    {
        for(uint64_t ii = 0; ii < img->md->nelement; ii++)
        {
            img->im->array.F[ii] = (float) ran1();
        }
    }
    if(pdf == 1)
    {
        for(uint64_t ii = 0; ii < img->md->nelement; ii++)
        {
            img->im->array.F[ii] = (float) gauss();
        }
    }
    if(pdf == 2)
    {
        for(uint64_t ii = 0; ii < img->md->nelement; ii++)
        {
            img->im->array.F[ii] = (float) gauss_trc();
        }
    }

    DEBUG_TRACE_FEXIT();
    return(img->ID);
}



static errno_t compute_function()
{
    DEBUG_TRACE_FSTART();

    DEBUG_TRACEPOINT("make IMGID for %s", outim.name);
    IMGID img = makeIMGID_2D(outim.name, *outim.xsize, *outim.ysize);
    img.shared = *outim.shared;
    img.NBkw   = *outim.NBkw;
    img.CBsize = *outim.CBsize;


    INSERT_STD_PROCINFO_COMPUTEFUNC_START

    make_image_random(
        &img,
        *distrib
    );

    DEBUG_TRACEPOINT("update output ID %ld", img.ID);
    processinfo_update_output_stream(processinfo, img.ID);
    INSERT_STD_PROCINFO_COMPUTEFUNC_END

    DEBUG_TRACE_FEXIT();
    return RETURN_SUCCESS;
}




INSERT_STD_FPSCLIfunctions

// Register function in CLI
errno_t CLIADDCMD_image_gen__mkrandomim()
{
    INSERT_STD_CLIREGISTERFUNC
    return RETURN_SUCCESS;
}

