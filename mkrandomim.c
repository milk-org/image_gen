#include "CommandLineInterface/CLIcore.h"

#include "statistic/statistic.h"



// Local variables pointers
static char *outimname;
static uint32_t *imxsize;
static uint32_t *imysize;
static int *distrib;

static CLICMDARGDEF farg[] =
{
    {
        CLIARG_STR, ".out_name", "output image", "out1",
        CLIARG_VISIBLE_DEFAULT,
        (void **) &outimname
    },
    {
        CLIARG_LONG, ".xsize", "x size", "512",
        CLIARG_VISIBLE_DEFAULT,
        (void **) &imxsize
    },
    {
        CLIARG_LONG, ".ysize", "y size", "512",
        CLIARG_VISIBLE_DEFAULT,
        (void **) &imysize
    },
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








static imageID make_image_random(
    IMGID *img,
    int pdf
)
{
    uint32_t naxes[2];
    uint64_t nelement;

    // 0: uniform
    // 1: gauss
    // 2: truncated gauss


    // Create image if needed
    imcreateIMGID(img);


    naxes[0] = img->md->size[0];
    naxes[1] = img->md->size[1];
    nelement = naxes[0] * naxes[1];

    // openMP is slow when calling gsl random number generator : do not use openMP here
    if(pdf == 0)
    {
        for(uint64_t ii = 0; ii < nelement; ii++)
        {
            img->im->array.F[ii] = (float) ran1();
        }
    }
    if(pdf == 1)
    {
        for(uint64_t ii = 0; ii < nelement; ii++)
        {
            img->im->array.F[ii] = (float) gauss();
        }
    }
    if(pdf == 2)
    {
        for(uint64_t ii = 0; ii < nelement; ii++)
        {
            img->im->array.F[ii] = (float) gauss_trc();
        }
    }
    if(pdf == 3)
    {
        float gain = 0.01;
        for(uint32_t ii = 1; ii < naxes[0] - 1; ii++)
        {
            for(uint32_t jj = 1; jj < naxes[1] - 1; jj++)
            {
                float val = img->im->array.F[(jj) * naxes[0] + (ii)];
                float proxval = 0;
                proxval += img->im->array.F[(jj - 1) * naxes[0] + (ii - 1)];
                proxval += img->im->array.F[(jj - 1) * naxes[0] + (ii)];
                proxval += img->im->array.F[(jj - 1) * naxes[0] + (ii + 1)];
                proxval += img->im->array.F[(jj) * naxes[0] + (ii - 1)];
                proxval += img->im->array.F[(jj) * naxes[0] + (ii + 1)];
                proxval += img->im->array.F[(jj + 1) * naxes[0] + (ii - 1)];
                proxval += img->im->array.F[(jj + 1) * naxes[0] + (ii)];
                proxval += img->im->array.F[(jj + 1) * naxes[0] + (ii - 1)];

                if(val > 0.5) // live cell
                {
                    if(proxval < 2.0)
                    {
                        val -= gain;
                    }
                    if(proxval > 3.0)
                    {
                        val -= gain;
                    }
                }
                else // dead cell
                {
                    if(proxval > 3.0)
                    {
                        val += gain;
                    }
                }
                val += (float) gauss() * 0.1 * gain;
                if(val < 0.0)
                {
                    val = 0.0;
                }
                if(val > 5.0)
                {
                    val = 5.0;
                }
                img->im->array.F[(jj) * naxes[0] + (ii)] = val;
            }
        }
    }

    DEBUG_TRACEPOINT(" ");

    return(img->ID);
}



static errno_t compute_function()
{
    DEBUG_TRACEPOINT(" ");


    printf("image \"%s\"\n", outimname);

    IMGID img = makeIMGID(outimname);
    img.naxis = 2;
    img.size[0] = *imxsize;
    img.size[1] = *imysize;

    INSERT_STD_PROCINFO_COMPUTEFUNC_START

    DEBUG_TRACEPOINT("starting random image func");
    make_image_random(
        &img,
        *distrib
    );

    DEBUG_TRACEPOINT("updating output");


    processinfo_update_output_stream(processinfo, img.ID);
    INSERT_STD_PROCINFO_COMPUTEFUNC_END

    DEBUG_TRACEPOINT(" ");

    return RETURN_SUCCESS;
}


INSERT_STD_FPSCLIfunctions

// Register function in CLI
errno_t CLIADDCMD_image_gen__mkrandomim()
{
    INSERT_STD_CLIREGISTERFUNC

    return RETURN_SUCCESS;
}

