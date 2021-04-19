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
    const char *ID_name,
    uint32_t    l1,
    uint32_t    l2,
    int pdf
)
{
    imageID  ID = -1;
    uint32_t naxes[2];
    uint64_t nelement;

    if(pdf == 0)
    {
        // uniform
        printf("uniform distribution\n");
    }

    if(pdf == 1)
    {
        // gauss
        printf("gaussian distribution\n");
    }

    if(pdf == 2)
    {
        // truncated gauss
        printf("truncated gaussian distribution\n");
    }

    //fprintf(stdout, "Image size = %u %u\n", l1, l2);

    // Create image
    DEBUG_TRACEPOINT(" ");
    long naxis = 2;
    uint32_t *sizearray = (uint32_t *) malloc(sizeof(uint32_t) * naxis);
    sizearray[0] = l1;
    sizearray[1] = l2;

    uint8_t datatype = _DATATYPE_FLOAT;
    int shared = 0;
    int NBkw = 10;
    DEBUG_TRACEPOINT(" ");
    ID = create_image_ID(ID_name, naxis, sizearray, datatype, shared, NBkw);
    DEBUG_TRACEPOINT(" ");
    free(sizearray);

    //create_2Dimage_ID(ID_name, l1, l2);

    //ID = image_ID(ID_name);


    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];
    nelement = naxes[0] * naxes[1];


    // openMP is slow when calling gsl random number generator : do not use openMP here
    if(pdf == 0)
    {
        for(uint64_t ii = 0; ii < nelement; ii++)
        {
            data.image[ID].array.F[ii] = (float) ran1();
        }
    }
    if(pdf == 1)
    {
        for(uint64_t ii = 0; ii < nelement; ii++)
        {
            data.image[ID].array.F[ii] = (float) gauss();
        }
    }
    if(pdf == 2)
    {
        for(uint64_t ii = 0; ii < nelement; ii++)
        {
            data.image[ID].array.F[ii] = (float) gauss_trc();
        }
    }

    return(ID);
}



static errno_t compute_function()
{
    DEBUG_TRACEPOINT(" ");
    //IMGID outimg = makeIMGID(outimname);
    //resolveIMGID(&outimg, ERRMODE_ABORT);

    INSERT_STD_PROCINFO_COMPUTEFUNC_START

    DEBUG_TRACEPOINT(" ");
    make_image_random(
        outimname,
        *imxsize,
        *imysize,
        *distrib
    );

    //processinfo_update_output_stream(processinfo, outimg.ID);

    INSERT_STD_PROCINFO_COMPUTEFUNC_END

    return RETURN_SUCCESS;
}


INSERT_STD_FPSCLIfunctions

// Register function in CLI
errno_t CLIADDCMD_image_gen__mkrandomim()
{
    INSERT_STD_CLIREGISTERFUNC

    return RETURN_SUCCESS;
}

