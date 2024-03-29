/**
 * @file    image_gen.c
 * @brief   Generate frequently used image(s)
 *
 * Creates images for misc applications
 *
 */

/* ================================================================== */
/* ================================================================== */
/*            MODULE INFO                                             */
/* ================================================================== */
/* ================================================================== */

// module default short name
// all CLI calls to this module functions will be <shortname>.<funcname>
// if set to "", then calls use <funcname>
#define MODULE_SHORTNAME_DEFAULT "imgen"

// Module short description
#define MODULE_DESCRIPTION                                                     \
    "Creating images (shapes, useful functions and patterns)"

#include <malloc.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <fitsio.h> /* required by every program that uses CFITSIO  */

#include "CommandLineInterface/CLIcore.h"

#include "COREMOD_arith/COREMOD_arith.h"
#include "COREMOD_iofits/COREMOD_iofits.h"
#include "COREMOD_memory/COREMOD_memory.h"

#include "statistic/statistic.h"

#include "image_gen/image_gen.h"

#include "mkrandomim.h"

#define OMP_NELEMENT_LIMIT 1000000

#define SWAP(x, y)                                                             \
    tmp = (x);                                                                 \
    x   = (y);                                                                 \
    y   = tmp;

#define PI 3.14159265358979323846264338328

/* ================================================================== */
/* ================================================================== */
/*            INITIALIZE LIBRARY                                      */
/* ================================================================== */
/* ================================================================== */

// Module initialization macro in CLIcore.h
// macro argument defines module name for bindings
//
INIT_MODULE_LIB(image_gen)

/* ================================================================== */
/* ================================================================== */
/*            COMMAND LINE INTERFACE (CLI) FUNCTIONS                  */
/* ================================================================== */
/* ================================================================== */

errno_t make_disk_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_INT64) + CLI_checkarg(4, CLIARG_FLOAT64) +
            CLI_checkarg(5, CLIARG_FLOAT64) + CLI_checkarg(6, CLIARG_FLOAT64) ==
            0)
    {
        make_disk(data.cmdargtoken[1].val.string,
                  data.cmdargtoken[2].val.numl,
                  data.cmdargtoken[3].val.numl,
                  data.cmdargtoken[4].val.numf,
                  data.cmdargtoken[5].val.numf,
                  data.cmdargtoken[6].val.numf);
        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t make_subpixdisk_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_INT64) + CLI_checkarg(4, CLIARG_FLOAT64) +
            CLI_checkarg(5, CLIARG_FLOAT64) + CLI_checkarg(6, CLIARG_FLOAT64) ==
            0)
    {
        make_subpixdisk(data.cmdargtoken[1].val.string,
                        data.cmdargtoken[2].val.numl,
                        data.cmdargtoken[3].val.numl,
                        data.cmdargtoken[4].val.numf,
                        data.cmdargtoken[5].val.numf,
                        data.cmdargtoken[6].val.numf);
        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t make_gauss_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_INT64) + CLI_checkarg(4, CLIARG_FLOAT64) +
            CLI_checkarg(5, CLIARG_FLOAT64) ==
            0)
    {
        make_gauss(data.cmdargtoken[1].val.string,
                   data.cmdargtoken[2].val.numl,
                   data.cmdargtoken[3].val.numl,
                   data.cmdargtoken[4].val.numf,
                   data.cmdargtoken[5].val.numf);
        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t make_FiberCouplingOverlap_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) == 0)
    {
        make_FiberCouplingOverlap(data.cmdargtoken[1].val.string);
        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t make_slopexy_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_INT64) + CLI_checkarg(4, CLIARG_FLOAT64) +
            CLI_checkarg(5, CLIARG_FLOAT64) ==
            0)
    {
        make_slopexy(data.cmdargtoken[1].val.string,
                     data.cmdargtoken[2].val.numl,
                     data.cmdargtoken[3].val.numl,
                     data.cmdargtoken[4].val.numf,
                     data.cmdargtoken[5].val.numf);
        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t make_dist_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_INT64) + CLI_checkarg(4, CLIARG_FLOAT64) +
            CLI_checkarg(5, CLIARG_FLOAT64) ==
            0)
    {
        make_dist(data.cmdargtoken[1].val.string,
                  data.cmdargtoken[2].val.numl,
                  data.cmdargtoken[3].val.numl,
                  data.cmdargtoken[4].val.numf,
                  data.cmdargtoken[5].val.numf);
        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t make_hexsegpupil_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_FLOAT64) + CLI_checkarg(4, CLIARG_FLOAT64) +
            CLI_checkarg(5, CLIARG_FLOAT64) ==
            0)
    {
        make_hexsegpupil(data.cmdargtoken[1].val.string,
                         data.cmdargtoken[2].val.numl,
                         data.cmdargtoken[3].val.numf,
                         data.cmdargtoken[4].val.numf,
                         data.cmdargtoken[5].val.numf);
        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t IMAGE_gen_segments2WFmodes_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_IMG) ==
            0)
    {
        IMAGE_gen_segments2WFmodes(data.cmdargtoken[1].val.string,
                                   data.cmdargtoken[2].val.numl,
                                   data.cmdargtoken[3].val.string);
        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t make_rectangle_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_INT64) + CLI_checkarg(4, CLIARG_FLOAT64) +
            CLI_checkarg(5, CLIARG_FLOAT64) + CLI_checkarg(6, CLIARG_FLOAT64) +
            CLI_checkarg(7, CLIARG_FLOAT64) ==
            0)
    {
        make_rectangle(data.cmdargtoken[1].val.string,
                       data.cmdargtoken[2].val.numl,
                       data.cmdargtoken[3].val.numl,
                       data.cmdargtoken[4].val.numf,
                       data.cmdargtoken[5].val.numf,
                       data.cmdargtoken[6].val.numf,
                       data.cmdargtoken[7].val.numf);
        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t make_line_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_INT64) + CLI_checkarg(4, CLIARG_FLOAT64) +
            CLI_checkarg(5, CLIARG_FLOAT64) + CLI_checkarg(6, CLIARG_FLOAT64) +
            CLI_checkarg(7, CLIARG_FLOAT64) + CLI_checkarg(8, CLIARG_FLOAT64) ==
            0)
    {
        make_line(data.cmdargtoken[1].val.string,
                  data.cmdargtoken[2].val.numl,
                  data.cmdargtoken[3].val.numl,
                  data.cmdargtoken[4].val.numf,
                  data.cmdargtoken[5].val.numf,
                  data.cmdargtoken[6].val.numf,
                  data.cmdargtoken[7].val.numf,
                  data.cmdargtoken[8].val.numf);
        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t make_lincoordinate_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_INT64) + CLI_checkarg(4, CLIARG_FLOAT64) +
            CLI_checkarg(5, CLIARG_FLOAT64) + CLI_checkarg(6, CLIARG_FLOAT64) ==
            0)
    {
        make_lincoordinate(data.cmdargtoken[1].val.string,
                           data.cmdargtoken[2].val.numl,
                           data.cmdargtoken[3].val.numl,
                           data.cmdargtoken[4].val.numf,
                           data.cmdargtoken[5].val.numf,
                           data.cmdargtoken[6].val.numf);
        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t make_2Dgridpix_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_INT64) + CLI_checkarg(4, CLIARG_FLOAT64) +
            CLI_checkarg(5, CLIARG_FLOAT64) + CLI_checkarg(6, CLIARG_FLOAT64) +
            CLI_checkarg(7, CLIARG_FLOAT64) ==
            0)
    {
        make_2Dgridpix(data.cmdargtoken[1].val.string,
                       data.cmdargtoken[2].val.numl,
                       data.cmdargtoken[3].val.numl,
                       data.cmdargtoken[4].val.numf,
                       data.cmdargtoken[5].val.numf,
                       data.cmdargtoken[6].val.numf,
                       data.cmdargtoken[7].val.numf);

        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t make_rnd_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_INT64) ==
            0)
    {
        make_rnd(data.cmdargtoken[1].val.string,
                 data.cmdargtoken[2].val.numl,
                 data.cmdargtoken[3].val.numl,
                 "");

        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t make_rndgauss_cli()
{
    if(CLI_checkarg(1, CLIARG_STR_NOT_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_INT64) ==
            0)
    {

        make_rnd(data.cmdargtoken[1].val.string,
                 data.cmdargtoken[2].val.numl,
                 data.cmdargtoken[3].val.numl,
                 "gauss");

        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t image_gen_im2coord_cli()
{
    if(CLI_checkarg(1, CLIARG_IMG) + CLI_checkarg(2, CLIARG_INT64) +
            CLI_checkarg(3, CLIARG_STR_NOT_IMG) ==
            0)
    {

        image_gen_im2coord(data.cmdargtoken[1].val.string,
                           data.cmdargtoken[2].val.numl,
                           data.cmdargtoken[3].val.string);

        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

errno_t image_gen_make_voronoi_map_cli()
{
    if(CLI_checkarg(1, CLIARG_STR) + CLI_checkarg(2, CLIARG_STR_NOT_IMG) +
            CLI_checkarg(3, CLIARG_INT64) + CLI_checkarg(4, CLIARG_INT64) +
            CLI_checkarg(5, CLIARG_FLOAT64) + CLI_checkarg(6, CLIARG_FLOAT64) ==
            0)
    {

        image_gen_make_voronoi_map(data.cmdargtoken[1].val.string,
                                   data.cmdargtoken[2].val.string,
                                   data.cmdargtoken[3].val.numl,
                                   data.cmdargtoken[4].val.numl,
                                   data.cmdargtoken[5].val.numf,
                                   data.cmdargtoken[6].val.numf);

        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}

static errno_t init_module_CLI()
{

    RegisterCLIcommand(
        "mkdisk",
        __FILE__,
        make_disk_cli,
        "make disk image",
        "<output image name> <xsize> <yize> <xcenter> <ycenter> <radius>",
        "mkdisk imdisk 512 512 256.0 256.0 100.0",
        "long make_disk(const char *ID_name, long l1, long l2, double "
        "x_center, double y_center, double radius)");

    RegisterCLIcommand(
        "mkspdisk",
        __FILE__,
        make_subpixdisk_cli,
        "make disk image with sub-pixel interpolation",
        "<output image name> <xsize> <yize> <xcenter> <ycenter> <radius>",
        "mkspdisk imdisk 512 512 256.0 256.0 100.0",
        "long make_subpixdisk(const char *ID_name, long l1, long l2, double "
        "x_center, double y_center, double radius)");

    RegisterCLIcommand("mkgauss",
                       __FILE__,
                       make_gauss_cli,
                       "make gaussian spot image, A*exp(-r*r/a/a). FWHM : "
                       "ln(2)=r*r/a/a : r*r = a*a*ln(2) : r = a*sqrt(ln(2))",
                       "<output image name> <xsize> <yize> <a> <A>",
                       "mkgauss imdisk 512 512 12.0 1.0",
                       "long make_gauss(const char *ID_name, long l1, long l2, "
                       "double a, double A)");

    RegisterCLIcommand(
        "mkfiberclpoverlap",
        __FILE__,
        make_FiberCouplingOverlap_cli,
        "make fiber coupling overlap integral",
        "<output image name>",
        "mkfibercpl imdisk",
        "imageID make_FiberCouplingOverlap(const char *ID_name)");

    RegisterCLIcommand("mkslopexy",
                       __FILE__,
                       make_slopexy_cli,
                       "make slope image",
                       "<output image name> <xsize> <yize> <slopex> <slopey>",
                       "mkslope im 512 512 1.2 1.0",
                       "long make_slopexy(const char *ID_name, long l1,long "
                       "l2, double sx, double sy)");

    RegisterCLIcommand("mkdist",
                       __FILE__,
                       make_dist_cli,
                       "make image, pixel value = distance from point in image",
                       "<output image name> <xsize> <yize> <centerx> <centery>",
                       "mkdist im 512 512 256.0 256.0",
                       "long make_dist(const char *ID_name, long l1,long l2, "
                       "double f1, double f2)");

    RegisterCLIcommand("mkhexsegpup",
                       __FILE__,
                       make_hexsegpupil_cli,
                       "make hexagonal segmented pupil",
                       "<output image name> <size> <radius> <gap> <step>",
                       "mkhexsegpup imhex 4096 200 2.0 46.3",
                       "long make_hexsegpupil(const char *IDname, long size, "
                       "double radius, double gap, double step)");

    RegisterCLIcommand(
        "segs2wfmodes",
        __FILE__,
        IMAGE_gen_segments2WFmodes_cli,
        "make WF modes from TT&piston of segments. Segment names: <prefix>xxx",
        "<seg image prefix> <number of digits> <output image name>",
        "seg2wfmodes segim 2 WFmodes",
        "ong IMAGE_gen_segments2WFmodes(const char *prefix, long ndigit, const "
        "char *IDout)");

    RegisterCLIcommand("mkrect",
                       __FILE__,
                       make_rectangle_cli,
                       "make rectangle",
                       "<output image name> <xsize> <ysize> <xcenter> "
                       "<ycenter> <radius1> <radius2>",
                       "mkrect 512 512 256.0 256.0 100.0 200.0",
                       "long make_rectangle(const char *ID_name, long l1, long "
                       "l2, double x_center, double y_center, "
                       "double radius1, double radius2)");

    RegisterCLIcommand(
        "mkline",
        __FILE__,
        make_line_cli,
        "make line",
        "<output image name> <xsize> <ysize> <x1> <y1> <x2> <y2> <thickness>",
        "mkline lim 512 512 256.0 256.0 100.0 200.0 3.0",
        "long make_line(const char *IDname, long l1, long l2, double x1, "
        "double y1, double x2, double y2, double t)");

    RegisterCLIcommand("mklincoord",
                       __FILE__,
                       make_lincoordinate_cli,
                       "make linear coordinate",
                       "<output image name> <xsize> <ysize> <xc> <yc> <angle>",
                       "mklincoord lim 512 512 256.0 256.0 1.42",
                       "long make_lincoordinate(const char *IDname, long l1, "
                       "long l2, double x_center, double "
                       "y_center, double angle)");

    RegisterCLIcommand("mkgridpix",
                       __FILE__,
                       make_2Dgridpix_cli,
                       "make regular grid",
                       "<output image name> <xsize> <ysize> <xpitch> <ypitch> "
                       "<xoffset> <yoffset>",
                       "mkgridpix impgrid 512 512 10.0 10.0 4.5 2.8",
                       "long make_2Dgridpix(const char *IDname, long xsize, "
                       "long ysize, double pitchx, double pitchy, "
                       "double offsetx, double offsety)");

    RegisterCLIcommand("mkrndim",
                       __FILE__,
                       make_rnd_cli,
                       "make random image",
                       "<name> <xsize> <ysize>",
                       "mkrndim im 512 512",
                       "long make_rnd(const char *ID_name, long l1, long l2, "
                       "const char *options)");

    RegisterCLIcommand("mkrndgim",
                       __FILE__,
                       make_rndgauss_cli,
                       "make random image, gaussian distrib",
                       "<name> <xsize> <ysize>",
                       "mkrndgim im 512 512",
                       "long make_rnd(const char *ID_name, long l1, long l2, "
                       "const char *options)");

    RegisterCLIcommand("im2coord",
                       __FILE__,
                       image_gen_im2coord_cli,
                       "make coordinate image",
                       "<in> <axis> <out>",
                       "im2coord imin 1 imy",
                       "long image_gen_im2coord(const char *IDin_name, int "
                       "axis, const char *IDout_name)");

    RegisterCLIcommand(
        "mkvoronoi",
        __FILE__,
        image_gen_make_voronoi_map_cli,
        "make Voronoi map from points file",
        "<map file [ASCII]> <output map [IMG]> <xsize [LONG]> <ysize [LONG]> "
        "<radius [FLOAT]> <gap [FLOAT]>",
        "mkvoronoi voronoi.pts vmapim 256 256 0.1 0.01",
        "long image_gen_make_voronoi_map(const char *filename, const char "
        "*IDout_name, uint32_t xsize, uint32_t ysize, "
        "float radius, float maxsep)");

    CLIADDCMD_image_gen__mkrandomim();

    //long make_rnd(const char *ID_name, long l1, long l2, const char *options)

    // add atexit functions here

    return RETURN_SUCCESS;
}

/** @brief creates a double star */
imageID make_double_star(const char *ID_name,
                         uint32_t    l1,
                         uint32_t    l2,
                         double      intensity_1,
                         double      intensity_2,
                         double      separation,
                         double      position_angle)
{
    imageID  ID;
    uint32_t naxes[2];

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    data.image[ID]
    .array.F[((int)(naxes[1] / 2)) * naxes[0] + ((int)(naxes[0] / 2))] =
        intensity_1;
    data.image[ID]
    .array.F[((int)(naxes[1] / 2 + separation * cos(position_angle))) *
                             naxes[0] +
                             ((int)(naxes[0] / 2 + separation * sin(position_angle)))] =
                 intensity_2;

    return (ID);
}

/** @brief creates a disk */
imageID make_disk(const char *ID_name,
                  uint32_t    l1,
                  uint32_t    l2,
                  double      x_center,
                  double      y_center,
                  double      radius)
{
    imageID  ID;
    uint32_t ii, jj;
    uint32_t naxes[2];
    long     x1, x2, y1, y2;
    long     x1i, x2i, y1i, y2i;
    double   r2;
    /*
      int i,j;
      double r;
      double tot;
      int subgrid=100;
      double x,y;
    */

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    x1  = (long)(x_center - radius - 2);
    x2  = (long)(x_center + radius + 2);
    y1  = (long)(y_center - radius - 2);
    y2  = (long)(y_center + radius + 2);
    x1i = (long)(x_center - 0.707106781 * radius + 2);
    x2i = (long)(x_center + 0.707106781 * radius - 2);
    y1i = (long)(y_center - 0.707106781 * radius + 2);
    y2i = (long)(y_center + 0.707106781 * radius - 2);

    if(x1 < 0)
    {
        x1 = 0;
    }
    if(x1 > naxes[0])
    {
        x1 = naxes[0];
    }

    if(x2 < 0)
    {
        x2 = 0;
    }
    if(x2 > naxes[0])
    {
        x2 = naxes[0];
    }

    if(y1 < 0)
    {
        y1 = 0;
    }
    if(y1 > naxes[1])
    {
        y1 = naxes[1];
    }

    if(y2 > naxes[1])
    {
        y2 = naxes[1];
    }

    if(x1i < 0)
    {
        x1i = 0;
    }
    if(x1i > naxes[0])
    {
        x1i = naxes[0];
    }

    if(x2i < 0)
    {
        x2i = 0;
    }
    if(x2i > naxes[0])
    {
        x2i = naxes[0];
    }

    if(y1i < 0)
    {
        y1i = 0;
    }
    if(y1i > naxes[1])
    {
        y1i = naxes[1];
    }

    if(y2i < 0)
    {
        y2i = 0;
    }
    if(y2i > naxes[1])
    {
        y2i = naxes[1];
    }

    r2 = radius * radius;

    for(ii = x1i; ii < x2i; ii++)
        for(jj = y1i; jj < y2i; jj++)
        {
            data.image[ID].array.F[jj * naxes[0] + ii] = 1;
        }

    for(ii = x1; ii < x1i; ii++)
        for(jj = y1; jj < y2; jj++)
            if(((ii - x_center) * (ii - x_center) +
                    (jj - y_center) * (jj - y_center)) < r2)
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1;
            }

    for(ii = x2i; ii < x2; ii++)
        for(jj = y1; jj < y2; jj++)
            if(((ii - x_center) * (ii - x_center) +
                    (jj - y_center) * (jj - y_center)) < r2)
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1;
            }

    for(ii = x1i; ii < x2i; ii++)
        for(jj = y1; jj < y1i; jj++)
            if(((ii - x_center) * (ii - x_center) +
                    (jj - y_center) * (jj - y_center)) < r2)
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1;
            }

    for(ii = x1i; ii < x2i; ii++)
        for(jj = y2i; jj < y2; jj++)
            if(((ii - x_center) * (ii - x_center) +
                    (jj - y_center) * (jj - y_center)) < r2)
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1;
            }

    /*
    for (jj = x1; jj < x2; jj++)
      for (ii = y1; ii < y2; ii++)
        {
    if (((ii-x_center)*(ii-x_center)+(jj-y_center)*(jj-y_center))<r2)
      data.image[ID].array.F[jj*naxes[0]+ii] = 1;
        }
    */
    /*
      for (jj = 0; jj < naxes[1]; jj++)
      for (ii = 0; ii < naxes[0]; ii++)
      {
      r = sqrt(((ii-x_center)*(ii-x_center)+(jj-y_center)*(jj-y_center)));

      if (r<radius)
      data.image[ID].array.F[jj*naxes[0]+ii] = 1.0;
      else
      data.image[ID].array.F[jj*naxes[0]+ii] = 0.0;

      if(((radius-r)*(radius-r))<1.5)
      {
      tot = 0;
      for (j = 0; j < subgrid; j++)
      for (i = 0; i < subgrid; i++)
      {
      x = 1.0*ii-0.5+0.5/subgrid+1.0*i/subgrid;
      y = 1.0*jj-0.5+0.5/subgrid+1.0*j/subgrid;
      r = sqrt((x-1.0*x_center)*(x-1.0*x_center)+(y-1.0*y_center)*(y-1.0*y_center));
      if (r < radius)
      tot = tot + 1.0;
      else
      tot = tot + 0.0;
      }
      tot = tot/subgrid/subgrid;
      data.image[ID].array.F[jj*naxes[0]+ii] = tot;
      }
      }
    */
    return (ID);
}

imageID make_subpixdisk(const char *ID_name,
                        uint32_t    l1,
                        uint32_t    l2,
                        double      x_center,
                        double      y_center,
                        double      radius)
{
    imageID  ID;
    uint32_t ii, jj;
    uint32_t naxes[2];
    int      i, j;
    double   r;
    double   tot;
    int      subgrid = 55;
    double   grid[55]; // same number of points as subgrid
    double   x, y;
    long     x1, x2, y1, y2;
    long     x1i, x2i, y1i, y2i;
    double   r2, r2ref;
    double   xdiff, ydiff;
    double   subgrid2;

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    x1  = (long)(x_center - radius - 2);
    x2  = (long)(x_center + radius + 2);
    y1  = (long)(y_center - radius - 2);
    y2  = (long)(y_center + radius + 2);
    x1i = (long)(x_center - 0.707106781 * radius + 2);
    x2i = (long)(x_center + 0.707106781 * radius - 2);
    y1i = (long)(y_center - 0.707106781 * radius + 2);
    y2i = (long)(y_center + 0.707106781 * radius - 2);

    if(x1 < 0)
    {
        x1 = 0;
    }
    if(x1 > naxes[0])
    {
        x1 = naxes[0];
    }
    if(x2 < 0)
    {
        x2 = 0;
    }
    if(x2 > naxes[0])
    {
        x2 = naxes[0];
    }

    if(y1 < 0)
    {
        y1 = 0;
    }
    if(y1 > naxes[1])
    {
        y1 = naxes[1];
    }
    if(y2 < 0)
    {
        y2 = 0;
    }
    if(y2 > naxes[1])
    {
        y2 = naxes[1];
    }

    if(x1i < 0)
    {
        x1i = 0;
    }
    if(x1i > naxes[0] - 1)
    {
        x1i = naxes[0] - 1;
    }
    if(x2i < 0)
    {
        x2i = 0;
    }
    if(x2i > naxes[0] - 1)
    {
        x2i = naxes[0] - 1;
    }

    if(y1i < 0)
    {
        y1i = 0;
    }
    if(y1i > naxes[1] - 1)
    {
        y1i = naxes[1] - 1;
    }
    if(y2i < 0)
    {
        y2i = 0;
    }
    if(y2i > naxes[1] - 1)
    {
        y2i = naxes[1] - 1;
    }

    r2ref    = radius * radius;
    subgrid2 = subgrid * subgrid;

    for(ii = x1i; ii < x2i; ii++)
        for(jj = y1i; jj < y2i; jj++)
        {
            data.image[ID].array.F[jj * naxes[0] + ii] = 1;
        }

    for(i = 0; i < subgrid; i++)
    {
        grid[i] = (0.5 - 0.5 / subgrid - 1.0 * i / subgrid);
    }

    for(ii = x1; ii < x1i; ii++)
        for(jj = y1; jj < y2; jj++)
        {
            xdiff = x_center - ii;
            ydiff = y_center - jj;
            r2    = xdiff * xdiff + ydiff * ydiff;
            if(r2 < r2ref)
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1.0;
            }
            if(fabs(sqrt(r2) - sqrt(r2ref)) < 1.5)
            {
                tot = 0;
                for(j = 0; j < subgrid; j++)
                    for(i = 0; i < subgrid; i++)
                    {
                        x = xdiff + grid[i];
                        y = ydiff + grid[j];
                        r = x * x + y * y;
                        if(r < r2ref)
                        {
                            tot += 1.0;
                        }
                    }
                tot                                        = tot / subgrid2;
                data.image[ID].array.F[jj * naxes[0] + ii] = tot;
            }
        }

    for(ii = x2i; ii < x2; ii++)
        for(jj = y1; jj < y2; jj++)
        {
            xdiff = x_center - ii;
            ydiff = y_center - jj;
            r2    = xdiff * xdiff + ydiff * ydiff;
            if(r2 < r2ref)
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1.0;
            }
            if(fabs(sqrt(r2) - sqrt(r2ref)) < 1.5)
            {
                tot = 0;
                for(j = 0; j < subgrid; j++)
                    for(i = 0; i < subgrid; i++)
                    {
                        x = xdiff + grid[i];
                        y = ydiff + grid[j];
                        r = x * x + y * y;
                        if(r < r2ref)
                        {
                            tot += 1.0;
                        }
                    }
                tot                                        = tot / subgrid2;
                data.image[ID].array.F[jj * naxes[0] + ii] = tot;
            }
        }

    for(ii = x1i; ii < x2i; ii++)
        for(jj = y1; jj < y1i; jj++)
        {
            xdiff = x_center - ii;
            ydiff = y_center - jj;
            r2    = xdiff * xdiff + ydiff * ydiff;
            if(r2 < r2ref)
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1.0;
            }
            if(fabs(sqrt(r2) - sqrt(r2ref)) < 1.5)
            {
                tot = 0;
                for(j = 0; j < subgrid; j++)
                    for(i = 0; i < subgrid; i++)
                    {
                        x = xdiff + grid[i];
                        y = ydiff + grid[j];
                        r = x * x + y * y;
                        if(r < r2ref)
                        {
                            tot += 1.0;
                        }
                    }
                tot                                        = tot / subgrid2;
                data.image[ID].array.F[jj * naxes[0] + ii] = tot;
            }
        }

    for(ii = x1i; ii < x2i; ii++)
        for(jj = y2i; jj < y2; jj++)
        {
            xdiff = x_center - ii;
            ydiff = y_center - jj;
            r2    = xdiff * xdiff + ydiff * ydiff;
            if(r2 < r2ref)
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1.0;
            }
            if(fabs(sqrt(r2) - sqrt(r2ref)) < 1.5)
            {
                tot = 0;
                for(j = 0; j < subgrid; j++)
                    for(i = 0; i < subgrid; i++)
                    {
                        x = xdiff + grid[i];
                        y = ydiff + grid[j];
                        r = x * x + y * y;
                        if(r < r2ref)
                        {
                            tot += 1.0;
                        }
                    }
                tot                                        = tot / subgrid2;
                data.image[ID].array.F[jj * naxes[0] + ii] = tot;
            }
        }

    return (ID);
}

// creates a shape with contour described by sum of sine waves
//
// r = radius + SUM[ ra[i] * cos( ka[i]*PA/2.0/PI + pa[i]) ]

imageID make_subpixdisk_perturb(const char *ID_name,
                                uint32_t    l1,
                                uint32_t    l2,
                                double      x_center,
                                double      y_center,
                                double      radius,
                                long        n,
                                double     *ra,
                                double     *ka,
                                double     *pa)
{
    imageID  ID;
    uint32_t ii, jj;
    uint32_t naxes[2];
    int      i, j;
    double   r;
    double   tot;
    int      subgrid = 55;
    double   grid[55]; // same number of points as subgrid
    double   x, y;
    long     x1, x2, y1, y2;
    long     x1i, x2i, y1i, y2i;
    double   r2, r2ref;
    double   xdiff, ydiff;
    double   subgrid2;
    double   PA;
    double   v0;
    long     k;

    double radius1, radius2;

    radius1 = radius;
    radius2 = radius;
    for(k = 0; k < n; k++)
    {
        radius1 += radius * fabs(ra[k]);
    }
    for(k = 0; k < n; k++)
    {
        radius2 -= radius * fabs(ra[k]);
    }
    if(radius2 < 0.0)
    {
        radius2 = 0.0;
    }

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    x1  = (long)(x_center - radius1 - 2);
    x2  = (long)(x_center + radius1 + 2);
    y1  = (long)(y_center - radius1 - 2);
    y2  = (long)(y_center + radius1 + 2);
    x1i = (long)(x_center - 0.707106781 * radius2 + 2);
    x2i = (long)(x_center + 0.707106781 * radius2 - 2);
    y1i = (long)(y_center - 0.707106781 * radius2 + 2);
    y2i = (long)(y_center + 0.707106781 * radius2 - 2);

    if(x1 < 0)
    {
        x1 = 0;
    }
    if(x1 > naxes[0])
    {
        x1 = naxes[0];
    }
    if(x2 < 0)
    {
        x2 = 0;
    }
    if(x2 > naxes[0])
    {
        x2 = naxes[0];
    }

    if(y1 < 0)
    {
        y1 = 0;
    }
    if(y1 > naxes[1])
    {
        y1 = naxes[1];
    }
    if(y2 < 0)
    {
        y2 = 0;
    }
    if(y2 > naxes[1])
    {
        y2 = naxes[1];
    }

    if(x1i < 0)
    {
        x1i = 0;
    }
    if(x1i > naxes[0] - 1)
    {
        x1i = naxes[0] - 1;
    }
    if(x2i < 0)
    {
        x2i = 0;
    }
    if(x2i > naxes[0] - 1)
    {
        x2i = naxes[0] - 1;
    }

    if(y1i < 0)
    {
        y1i = 0;
    }
    if(y1i > naxes[1] - 1)
    {
        y1i = naxes[1] - 1;
    }
    if(y2i < 0)
    {
        y2i = 0;
    }
    if(y2i > naxes[1] - 1)
    {
        y2i = naxes[1] - 1;
    }

    r2ref    = radius * radius;
    subgrid2 = subgrid * subgrid;

    for(ii = x1i; ii < x2i; ii++)
        for(jj = y1i; jj < y2i; jj++)
        {
            data.image[ID].array.F[jj * naxes[0] + ii] = 1;
        }

    for(i = 0; i < subgrid; i++)
    {
        grid[i] = (0.5 - 0.5 / subgrid - 1.0 * i / subgrid);
    }

    for(ii = x1; ii < x1i; ii++)
        for(jj = y1; jj < y2; jj++)
        {
            xdiff = x_center - ii;
            ydiff = y_center - jj;
            PA    = atan2(ydiff, xdiff);
            r2    = xdiff * xdiff + ydiff * ydiff;

            v0 = radius;
            for(k = 0; k < n; k++)
            {
                v0 += radius * ra[k] * cos(ka[k] * PA + pa[k]);
            }
            r2ref = v0 * v0;

            if(r2 < r2ref)
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1.0;
            }
            if(fabs(sqrt(r2) - sqrt(r2ref)) < 1.5)
            {
                tot = 0;
                for(j = 0; j < subgrid; j++)
                    for(i = 0; i < subgrid; i++)
                    {
                        x  = xdiff + grid[i];
                        y  = ydiff + grid[j];
                        PA = atan2(y, x);
                        r  = x * x + y * y;

                        v0 = radius;
                        for(k = 0; k < n; k++)
                        {
                            v0 += radius * ra[k] * cos(ka[k] * PA + pa[k]);
                        }
                        r2ref = v0 * v0;

                        if(r < r2ref)
                        {
                            tot += 1.0;
                        }
                    }
                tot                                        = tot / subgrid2;
                data.image[ID].array.F[jj * naxes[0] + ii] = tot;
            }
        }

    for(ii = x2i; ii < x2; ii++)
        for(jj = y1; jj < y2; jj++)
        {
            xdiff = x_center - ii;
            ydiff = y_center - jj;
            PA    = atan2(ydiff, xdiff);
            r2    = xdiff * xdiff + ydiff * ydiff;

            v0 = radius;
            for(k = 0; k < n; k++)
            {
                v0 += radius * ra[k] * cos(ka[k] * PA + pa[k]);
            }
            r2ref = v0 * v0;

            if(r2 < r2ref)
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1.0;
            }
            if(fabs(sqrt(r2) - sqrt(r2ref)) < 1.5)
            {
                tot = 0;
                for(j = 0; j < subgrid; j++)
                    for(i = 0; i < subgrid; i++)
                    {
                        x  = xdiff + grid[i];
                        y  = ydiff + grid[j];
                        r  = x * x + y * y;
                        PA = atan2(y, x);
                        v0 = radius;
                        for(k = 0; k < n; k++)
                        {
                            v0 += radius * ra[k] * cos(ka[k] * PA + pa[k]);
                        }
                        r2ref = v0 * v0;

                        if(r < r2ref)
                        {
                            tot += 1.0;
                        }
                    }
                tot                                        = tot / subgrid2;
                data.image[ID].array.F[jj * naxes[0] + ii] = tot;
            }
        }

    for(ii = x1i; ii < x2i; ii++)
        for(jj = y1; jj < y1i; jj++)
        {
            xdiff = x_center - ii;
            ydiff = y_center - jj;
            PA    = atan2(ydiff, xdiff);
            r2    = xdiff * xdiff + ydiff * ydiff;
            v0    = radius;
            for(k = 0; k < n; k++)
            {
                v0 += radius * ra[k] * cos(ka[k] * PA + pa[k]);
            }
            r2ref = v0 * v0;

            if(r2 < r2ref)
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1.0;
            }
            if(fabs(sqrt(r2) - sqrt(r2ref)) < 1.5)
            {
                tot = 0;
                for(j = 0; j < subgrid; j++)
                    for(i = 0; i < subgrid; i++)
                    {
                        x  = xdiff + grid[i];
                        y  = ydiff + grid[j];
                        PA = atan2(y, x);
                        r  = x * x + y * y;
                        v0 = radius;
                        for(k = 0; k < n; k++)
                        {
                            v0 += radius * ra[k] * cos(ka[k] * PA + pa[k]);
                        }
                        r2ref = v0 * v0;
                        if(r < r2ref)
                        {
                            tot += 1.0;
                        }
                    }
                tot                                        = tot / subgrid2;
                data.image[ID].array.F[jj * naxes[0] + ii] = tot;
            }
        }

    for(ii = x1i; ii < x2i; ii++)
        for(jj = y2i; jj < y2; jj++)
        {
            xdiff = x_center - ii;
            ydiff = y_center - jj;
            PA    = atan2(ydiff, xdiff);
            r2    = xdiff * xdiff + ydiff * ydiff;
            v0    = radius;
            for(k = 0; k < n; k++)
            {
                v0 += radius * ra[k] * cos(ka[k] * PA + pa[k]);
            }
            r2ref = v0 * v0;

            if(r2 < r2ref)
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1.0;
            }
            if(fabs(sqrt(r2) - sqrt(r2ref)) < 1.5)
            {
                tot = 0;
                for(j = 0; j < subgrid; j++)
                    for(i = 0; i < subgrid; i++)
                    {
                        x  = xdiff + grid[i];
                        y  = ydiff + grid[j];
                        PA = atan2(y, x);
                        r  = x * x + y * y;
                        v0 = radius;
                        for(k = 0; k < n; k++)
                        {
                            v0 += radius * ra[k] * cos(ka[k] * PA + pa[k]);
                        }
                        r2ref = v0 * v0;
                        if(r < r2ref)
                        {
                            tot += 1.0;
                        }
                    }
                tot                                        = tot / subgrid2;
                data.image[ID].array.F[jj * naxes[0] + ii] = tot;
            }
        }

    return (ID);
}

/* creates a square */
imageID make_square(const char *ID_name,
                    uint32_t    l1,
                    uint32_t    l2,
                    double      x_center,
                    double      y_center,
                    double      radius)
{
    imageID  ID;
    uint32_t naxes[2];

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    for(uint32_t jj = 0; jj < naxes[1]; jj++)
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            if((((ii - x_center) * (ii - x_center)) < (radius * radius)) &&
                    (((jj - y_center) * (jj - y_center)) < (radius * radius)))
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1;
            }
        }

    return (ID);
}

imageID make_rectangle(const char *ID_name,
                       uint32_t    l1,
                       uint32_t    l2,
                       double      x_center,
                       double      y_center,
                       double      radius1,
                       double      radius2)
{
    imageID  ID;
    uint32_t naxes[2];

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    for(uint32_t jj = 0; jj < naxes[1]; jj++)
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            if((((ii - x_center) * (ii - x_center)) < (radius1 * radius1)) &&
                    (((jj - y_center) * (jj - y_center)) < (radius2 * radius2)))
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1;
            }
        }

    return (ID);
}

// line of thickness t from (x1,y1) to (x2,y2)
imageID make_line(const char *IDname,
                  uint32_t    l1,
                  uint32_t    l2,
                  double      x1,
                  double      y1,
                  double      x2,
                  double      y2,
                  double      t)
{
    imageID  ID;
    double   x, y, xr, yr, r0;
    double   PA0;
    uint32_t naxes[2];

    create_2Dimage_ID(IDname, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    r0  = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    PA0 = atan2((y2 - y1), (x2 - x1));
    for(uint32_t jj = 0; jj < naxes[1]; jj++)
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            x = 1.0 * ii;
            y = 1.0 * jj;
            x -= x1;
            y -= y1;
            xr = x * cos(PA0) + y * sin(PA0);
            yr = -x * sin(PA0) + y * cos(PA0);
            //r=sqrt(xr*xr+yr*yr);
            xr /= r0;
            yr /= r0;
            if((xr > 0) && (xr < 1.0) && (yr < 0.5 * t / r0) &&
                    (yr > -0.5 * t / r0))
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1.0;
            }
            else
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 0.0;
            }
        }

    return (ID);
}

// draw line crossing point xc, yc with angle, pixel value is coordinate axis perp to line
imageID make_lincoordinate(const char *IDname,
                           uint32_t    l1,
                           uint32_t    l2,
                           double      x_center,
                           double      y_center,
                           double      angle)
{
    imageID  ID;
    uint32_t naxes[2];
    double   x, y, x1;

    create_2Dimage_ID(IDname, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    for(uint32_t jj = 0; jj < naxes[1]; jj++)
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            x  = 1.0 * ii - x_center;
            y  = 1.0 * jj - y_center;
            x1 = x * cos(angle) + y * sin(angle);
            //y1 = -x*sin(angle) + y*cos(angle);
            data.image[ID].array.F[jj * naxes[0] + ii] = x1;
        }

    return (ID);
}

imageID make_hexagon(const char *IDname,
                     uint32_t    l1,
                     uint32_t    l2,
                     double      x_center,
                     double      y_center,
                     double      radius)
{
    imageID  ID;
    uint32_t ii, jj;
    uint32_t naxes[2];
    float    x, y, r;
    float    value;

    long  iimin, iimax, jjmin, jjmax;
    float radius1, radius0sq;

    radius1   = radius * 2.0 / sqrt(3.0);
    radius0sq = radius * radius;

    printf("Making hexagon at %f x %f\n", x_center, y_center);

    create_2Dimage_ID(IDname, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    iimin = (long)(x_center - radius1 - 1.0);
    if(iimin < 0)
    {
        iimin = 0;
    }
    if(iimin > l1 - 1)
    {
        iimin = l1 - 1;
    }

    iimax = (long)(x_center + radius1 + 1.0);
    if(iimax < 0)
    {
        iimax = 0;
    }
    if(iimax > l1 - 1)
    {
        iimax = l1 - 1;
    }

    jjmin = (long)(y_center - radius1 - 1.0);
    if(jjmin < 0)
    {
        jjmin = 0;
    }
    if(jjmin > l2 - 1)
    {
        jjmin = l2 - 1;
    }

    jjmax = (long)(y_center + radius1 + 1.0);
    if(jjmax < 0)
    {
        jjmax = 0;
    }
    if(jjmax > l2 - 1)
    {
        jjmax = l2 - 1;
    }

#ifdef HAVE_LIBGOMP
    #pragma omp parallel default(shared) private(ii, jj, value, x, y, r)
    {
        #pragma omp for
#endif

        for(jj = jjmin; jj < jjmax; jj++)
            for(ii = iimin; ii < iimax; ii++)
            {
                value = 1.0;
                x     = 1.0 * ii - x_center;
                y     = 1.0 * jj - y_center;

                if(x * x + y * y > radius0sq)
                {
                    r = y;
                    if(fabs(r) > radius)
                    {
                        value = 0.0;
                    }
                    else
                    {
                        r = cos(PI / 6.0) * x + sin(PI / 6.0) * y;
                        if(fabs(r) > radius)
                        {
                            value = 0.0;
                        }
                        else
                        {
                            r = cos(-PI / 6.0) * x + sin(-PI / 6.0) * y;
                            if(fabs(r) > radius)
                            {
                                value = 0.0;
                            }
                        }
                    }
                }
                data.image[ID].array.F[jj * naxes[0] + ii] = value;
            }
#ifdef HAVE_LIBGOMP
    }
#endif

    return (ID);
}

imageID IMAGE_gen_segments2WFmodes(const char *prefix,
                                   long        ndigit,
                                   const char *IDout_name)
{
    imageID IDout = -1;
    long    NBseg;
    long    seg;
    int     OK;
    char    imname[200];
    imageID IDarray[10000];
    long    ii, jj, kk, xsize, ysize, xysize;
    double  x, y;
    imageID IDmask;
    double *segxc;
    double *segyc;
    double *segsum;

    seg = 0;
    OK  = 1;
    while(OK == 1)
    {
        switch(ndigit)
        {

            case 1:
                sprintf(imname, "%s%01ld", prefix, seg);
                break;
            case 2:
                sprintf(imname, "%s%02ld", prefix, seg);
                break;
            case 3:
                sprintf(imname, "%s%03ld", prefix, seg);
                break;
            case 4:
                sprintf(imname, "%s%04ld", prefix, seg);
                break;
            case 5:
                sprintf(imname, "%s%05ld", prefix, seg);
                break;
            case 6:
                sprintf(imname, "%s%06ld", prefix, seg);
                break;

            default:
                printf("ERROR: Invalid number of didits\n");
                exit(0);
        }
        IDarray[seg] = image_ID(imname);
        if(IDarray[seg] != -1)
        {
            seg++;
        }
        else
        {
            OK = 0;
        }
    }
    NBseg = seg;
    printf("Processing %ld segments\n", NBseg);
    if(NBseg > 0)
    {
        xsize  = data.image[IDarray[0]].md[0].size[0];
        ysize  = data.image[IDarray[0]].md[0].size[1];
        xysize = xsize * ysize;

        segxc = (double *) malloc(sizeof(double) * NBseg);
        if(segxc == NULL)
        {
            PRINT_ERROR("malloc returns NULL pointer");
            abort();
        }

        segyc = (double *) malloc(sizeof(double) * NBseg);
        if(segyc == NULL)
        {
            PRINT_ERROR("malloc returns NULL pointer");
            abort();
        }

        segsum = (double *) malloc(sizeof(double) * NBseg);
        if(segsum == NULL)
        {
            PRINT_ERROR("malloc returns NULL pointer");
            abort();
        }

        create_2Dimage_ID("_pupmask", xsize, ysize, &IDmask);

        for(seg = 0; seg < NBseg; seg++)
        {
            segxc[seg]  = 0.0;
            segyc[seg]  = 0.0;
            segsum[seg] = 0.0;

            for(ii = 0; ii < xsize; ii++)
                for(jj = 0; jj < ysize; jj++)
                {
                    x = 1.0 * ii;
                    y = 1.0 * jj;
                    segxc[seg] +=
                        x * data.image[IDarray[seg]].array.F[jj * xsize + ii];
                    segyc[seg] +=
                        y * data.image[IDarray[seg]].array.F[jj * xsize + ii];
                    segsum[seg] +=
                        data.image[IDarray[seg]].array.F[jj * xsize + ii];

                    data.image[IDmask].array.F[jj * xsize + ii] +=
                        (1.0 + seg) *
                        data.image[IDarray[seg]].array.F[jj * xsize + ii];
                }
            segxc[seg] /= segsum[seg];
            segyc[seg] /= segsum[seg];
        }

        save_fits("_pupmask", "_pupmask.fits");

        //IDtmp = create_2Dimage_ID("_seg2wfm_tmp", xsize, ysize);
        create_3Dimage_ID(IDout_name, xsize, ysize, 3 * NBseg, &IDout);
        kk = 0;
        for(seg = 0; seg < NBseg; seg++)  // create modes one at a time
        {
            // piston seg
            for(ii = 0; ii < xsize; ii++)
                for(jj = 0; jj < xsize; jj++)
                {
                    data.image[IDout].array.F[kk * xysize + jj * xsize + ii] =
                        data.image[IDarray[seg]].array.F[jj * xsize + ii];
                }
            kk++;

            // Tip
            for(ii = 0; ii < xsize; ii++)
                for(jj = 0; jj < xsize; jj++)
                {
                    data.image[IDout].array.F[kk * xysize + jj * xsize + ii] =
                        data.image[IDarray[seg]].array.F[jj * xsize + ii] *
                        (1.0 * ii - segxc[seg]);
                }
            kk++;

            // Tilt
            for(ii = 0; ii < xsize; ii++)
                for(jj = 0; jj < xsize; jj++)
                {
                    data.image[IDout].array.F[kk * xysize + jj * xsize + ii] =
                        data.image[IDarray[seg]].array.F[jj * xsize + ii] *
                        (1.0 * jj - segyc[seg]);
                }
            kk++;
        }

        //delete_image_ID("_seg2wfm_tmp", DELETE_IMAGE_ERRMODE_WARNING);

        free(segxc);
        free(segyc);
        free(segsum);
    }

    return (IDout);
}

imageID make_hexsegpupil(
    const char *IDname, uint32_t size, double radius, double gap, double step)
{
    imageID  ID, ID1, IDp;
    long     x1, y1;
    double   x2, y2;
    imageID  IDdisk;
    uint32_t ii;
    double   tot = 0.0;
    long     size2;

    int    PISTONerr   = 0;
    int    errSEGindex = -1;
    double pampl;
    double piston;
    long   SEGcnt = 0;

    int   mkInfluenceFunctions = 1;
    long  IDif;
    int   seg;
    long  kk, jj;
    float xc, yc, tc;

    int    WriteCIF = 0;
    FILE  *fpmlevel;
    FILE  *fp       = NULL;
    FILE  *fp1      = NULL;
    double pixscale = 1.0;
    long   vID;
    double x, y;
    int    pt;

    long   IDmap1;
    long   index;
    double mapscalefactor = 1.037;
    long   size1;

    long *seglevel;
    long  i;
    long  tmpl1, tmpl2;
    int   segi;
    float segf;
    int   k;

    int *bitval;       // 0 or 1
    int  bitindex = 4; // 0 = MSB

    double vx, vy, rmsx, rmsy;

    if(WriteCIF == 1)
    {
        fp  = fopen("hexcoord.txt", "w");
        fp1 = fopen("hexcoord_pt.txt", "w");

        fprintf(fp, "DS 1 1 1;\n");
    }

    if((vID = variable_ID("pixscale")) != -1)
    {
        pixscale = data.variable[vID].value.f;
        printf("pixscale = %f\n", pixscale);
    }

    SEGcnt = 100;
    if((vID = variable_ID("SEGcnt")) != -1)
    {
        SEGcnt = (long)(0.1 + data.variable[vID].value.f);
        printf("SEGcnt = %ld\n", SEGcnt);
    }

    seglevel = (long *) malloc(sizeof(long) * SEGcnt);
    if(seglevel == NULL)
    {
        PRINT_ERROR("malloc returns NULL pointer");
        abort();
    }

    bitval = (int *) malloc(sizeof(int) * SEGcnt);
    if(bitval == NULL)
    {
        PRINT_ERROR("malloc returns NULL pointer");
        abort();
    }

    fpmlevel = fopen("fpm_level.txt", "r");
    if(fpmlevel != NULL)
    {
        for(i = 0; i < SEGcnt; i++)
        {
            int fscanfcnt = fscanf(fpmlevel, "%ld %ld\n", &tmpl1, &tmpl2);
            if(fscanfcnt == EOF)
            {
                if(ferror(fp))
                {
                    perror("fscanf");
                }
                else
                {
                    fprintf(stderr,
                            "Error: fscanf reached end of file, no matching "
                            "characters, no matching failure\n");
                }
                exit(EXIT_FAILURE);
            }
            else if(fscanfcnt != 2)
            {
                fprintf(stderr,
                        "Error: fscanf successfully matched and assigned %i "
                        "input items, 2 expected\n",
                        fscanfcnt);
                exit(EXIT_FAILURE);
            }

            seglevel[tmpl1 - 1] = tmpl2 + 15;
        }
        fclose(fpmlevel);
    }

    // SINGLE BIT
    for(i = 0; i < SEGcnt; i++)
    {
        printf("%5ld %5ld   ", i + 1, seglevel[i]);
        segf = 1.0 * seglevel[i] / 16.0;
        for(k = 0; k < 5; k++)
        {
            segi = (int) segf;
            printf(" %d", segi);
            segf -= segi;
            segf *= 2;

            if(k == bitindex)
            {
                bitval[i] = segi;
            }
        }
        printf("\n");
    }

    IDmap1 = image_ID("indexmap");
    size1  = data.image[IDmap1].md[0].size[0];

    size2 = size * size;

    ID = variable_ID("hexpupnoif");
    if(ID != -1)
    {
        mkInfluenceFunctions = 0;
    }

    ID = variable_ID("HEXPISTONerr");
    if(ID != -1)
    {
        PISTONerr = 1;
        pampl     = data.variable[ID].value.f;
        printf("Piston error = %f\n", pampl);
    }
    else
    {
        pampl = 0.0;
    }

    ID = variable_ID("HEXPISTONindex");
    if(ID != -1)
    {
        errSEGindex = (long)(data.variable[ID].value.f + 0.01);
        printf("SEGMENT INDEX = %ld\n", (long) errSEGindex);
    }

    create_2Dimage_ID(IDname, size, size, &ID);
    if(PISTONerr == 1)
    {
        create_2Dimage_ID("hexpupPha", size, size, &IDp);
    }

    IDdisk = make_disk("_TMPdisk", size, size, size / 2, size / 2, radius);
    for(ii = 0; ii < size2; ii++)
    {
        data.image[IDdisk].array.F[ii] = 1.0 - data.image[IDdisk].array.F[ii];
    }

    SEGcnt = 0;
    for(x1 = -(long)(2 * size / step); x1 < (long)(2 * size / step); x1++)
        for(y1 = -(long)(2 * size / step); y1 < (long)(2 * size / step);
                y1++)
        {
            x2 = step * x1 * 3;
            y2 = step * sqrt(3.0) * y1;

            if(sqrt(x2 * x2 + y2 * y2) < radius)
            {
                if(errSEGindex == -1)
                {
                    piston = pampl * (1.0 - 2.0 * ran1());
                }
                else
                {
                    if(errSEGindex == SEGcnt)
                    {
                        piston = pampl;
                    }
                    else
                    {
                        piston = 0.0;
                    }
                }
                printf("Hexagon %ld: ", SEGcnt);
                ID1 = make_hexagon("_TMPhex",
                                   size,
                                   size,
                                   0.5 * size + x2,
                                   0.5 * size + y2,
                                   (step - gap) * (sqrt(3.0) / 2.0));

                tot = 0.0;
                for(ii = 0; ii < size2; ii++)
                {
                    tot += data.image[ID1].array.F[ii] *
                           data.image[IDdisk].array.F[ii];
                }
                if(tot < 0.1)
                {
                    SEGcnt++;
                    if(WriteCIF == 1)
                    {
                        ii = (long)(0.5 * size1 + x2 * (0.5 * size1 / radius) *
                                    mapscalefactor);
                        jj = (long)(0.5 * size1 + y2 * (0.5 * size1 / radius) *
                                    mapscalefactor);
                        index = 0;
                        if(IDmap1 != -1)
                        {
                            index =
                                data.image[IDmap1].array.UI16[jj * size1 + ii];
                        }

                        //  fprintf(fp, "# hex%03ld     index%03ld   [ %f %f ] -> [ %f %f ]     [%4ld %4ld] %f\n", SEGcnt, index, x2, y2, 0.5*size+x2, 0.5*size+y2, ii, jj, radius);
                        if(bitval[index - 1] == 1)
                        {
                            fprintf(fp, "L %ld;\n", seglevel[index - 1]);
                            fprintf(fp, "P");
                            for(pt = 0; pt < 6; pt++)
                            {
                                x = pixscale *
                                    (x2 + 1.0 * cos(2.0 * M_PI * pt / 6) *
                                     (step - gap));
                                y = pixscale *
                                    (y2 + 1.0 * sin(2.0 * M_PI * pt / 6) *
                                     (step - gap));
                                fprintf(fp,
                                        " %ld,%ld",
                                        (long)(100.0 * x),
                                        (long)(100.0 * y));
                                fprintf(fp1,
                                        "%ld %ld\n",
                                        (long)(100.0 * x),
                                        (long)(100.0 * y));
                            }
                            fprintf(fp, ";\n");
                        }
                    }

                    if(PISTONerr == 1)
                    {
                        for(ii = 0; ii < size2; ii++)
                        {
                            data.image[ID].array.F[ii] +=
                                data.image[ID1].array.F[ii];
                        }
                    }
                    else
                    {
                        for(ii = 0; ii < size2; ii++)
                        {
                            data.image[ID].array.F[ii] +=
                                1.0 * SEGcnt * data.image[ID1].array.F[ii];
                        }
                    }

                    if(PISTONerr == 1)
                    {
                        for(ii = 0; ii < size2; ii++)
                        {
                            data.image[IDp].array.F[ii] +=
                                data.image[ID1].array.F[ii] * piston;
                        }
                    }
                }
                delete_image_ID("_TMPhex", DELETE_IMAGE_ERRMODE_WARNING);
            }

            x2 += step * 1.5;
            y2 += step * sqrt(3.0) / 2.0;
            if(sqrt(x2 * x2 + y2 * y2) < radius)
            {
                // piston = pampl*(1.0-2.0*ran1());
                if(errSEGindex == -1)
                {
                    piston = pampl * (1.0 - 2.0 * ran1());
                }
                else
                {
                    if(errSEGindex == SEGcnt)
                    {
                        piston = pampl;
                    }
                    else
                    {
                        piston = 0.0;
                    }
                }
                printf("Hexagon %ld: ", SEGcnt);
                ID1 = make_hexagon("_TMPhex",
                                   size,
                                   size,
                                   0.5 * size + x2,
                                   0.5 * size + y2,
                                   (step - gap) * (sqrt(3.0) / 2.0));
                tot = 0.0;
                for(ii = 0; ii < size2; ii++)
                {
                    tot += data.image[ID1].array.F[ii] *
                           data.image[IDdisk].array.F[ii];
                }
                if(tot < 0.1)
                {
                    SEGcnt++;

                    if(WriteCIF == 1)
                    {
                        ii = (long)(0.5 * size1 + x2 * (0.5 * size1 / radius) *
                                    mapscalefactor);
                        jj = (long)(0.5 * size1 + y2 * (0.5 * size1 / radius) *
                                    mapscalefactor);
                        index = 0;
                        if(IDmap1 != -1)
                        {
                            index =
                                data.image[IDmap1].array.UI16[jj * size1 + ii];
                        }

                        // fprintf(fp, "# hex%03ld     index%03ld   [ %f %f ] -> [ %f %f ]   [%4ld %4ld] %f\n", SEGcnt, index, x2, y2, 0.5*size+x2, 0.5*size+y2, ii, jj, radius);

                        if(bitval[index - 1] == 1)
                        {
                            fprintf(fp, "L %ld;\n", seglevel[index - 1]);
                            fprintf(fp, "P");
                            for(pt = 0; pt < 6; pt++)
                            {
                                x = pixscale *
                                    (x2 + 1.0 * cos(2.0 * M_PI * pt / 6) *
                                     (step - gap));
                                y = pixscale *
                                    (y2 + 1.0 * sin(2.0 * M_PI * pt / 6) *
                                     (step - gap));
                                fprintf(fp,
                                        " %ld,%ld",
                                        (long)(100.0 * x),
                                        (long)(100.0 * y));
                                fprintf(fp1,
                                        "%ld %ld\n",
                                        (long)(100.0 * x),
                                        (long)(100.0 * y));
                            }
                            fprintf(fp, ";\n");
                        }
                    }

                    if(PISTONerr == 1)
                    {
                        for(ii = 0; ii < size2; ii++)
                        {
                            data.image[ID].array.F[ii] +=
                                data.image[ID1].array.F[ii];
                        }
                    }
                    else
                        for(ii = 0; ii < size2; ii++)
                        {
                            data.image[ID].array.F[ii] +=
                                1.0 * SEGcnt * data.image[ID1].array.F[ii];
                        }

                    if(PISTONerr == 1)
                    {
                        for(ii = 0; ii < size2; ii++)
                        {
                            data.image[IDp].array.F[ii] +=
                                data.image[ID1].array.F[ii] * piston;
                        }
                    }
                }
                delete_image_ID("_TMPhex", DELETE_IMAGE_ERRMODE_WARNING);
            }
        }
    delete_image_ID("_TMPdisk", DELETE_IMAGE_ERRMODE_WARNING);

    printf("%ld segments\n", SEGcnt);

    if(WriteCIF == 1)
    {
        fprintf(fp, "DF;\n");
        fprintf(fp, "E\n");

        fclose(fp);
        fclose(fp1);
    }
    free(seglevel);
    free(bitval);

    if(mkInfluenceFunctions == 1)  // TT and focus for each segment
    {

        create_3Dimage_ID("hexpupif", size, size, 3 * SEGcnt, &IDif);
        for(seg = 0; seg < SEGcnt; seg++)
        {

            // piston
            kk = 3 * seg;
            xc = 0.0;
            yc = 0.0;
            tc = 0.0;
            for(ii = 0; ii < size; ii++)
                for(jj = 0; jj < size; jj++)
                {
                    if(fabs(data.image[ID].array.F[jj * size + ii] -
                            (seg + 1.0)) < 0.01)
                    {
                        data.image[IDif].array.F[kk * size2 + jj * size + ii] =
                            1.0;
                        xc += 1.0 * ii;
                        yc += 1.0 * jj;
                        tc += 1.0;
                    }
                }
            xc /= tc;
            yc /= tc;

            // tip and tilt
            rmsx = 0.0;
            rmsy = 0.0;
            for(ii = 0; ii < size; ii++)
                for(jj = 0; jj < size; jj++)
                {
                    if(fabs(data.image[ID].array.F[jj * size + ii] -
                            (seg + 1.0)) < 0.01)
                    {
                        vx = 1.0 * ii - xc;
                        data.image[IDif]
                        .array.F[(kk + 1) * size2 + jj * size + ii] = vx;
                        rmsx += vx * vx;

                        vy = 1.0 * jj - yc;
                        data.image[IDif]
                        .array.F[(kk + 2) * size2 + jj * size + ii] = vy;
                        rmsy += vy * vy;
                    }
                }
            for(ii = 0; ii < size2; ii++)
            {
                data.image[IDif].array.F[(kk + 1) * size2 + ii] *=
                    sqrt(tc / rmsx);
                data.image[IDif].array.F[(kk + 2) * size2 + ii] *=
                    sqrt(tc / rmsy);
            }
        }
    }

    return (ID);
}

imageID make_jacquinot_pupil(const char *ID_name,
                             uint32_t    l1,
                             uint32_t    l2,
                             double      x_center,
                             double      y_center,
                             double      width,
                             double      height)
{
    imageID  ID;
    uint32_t naxes[2];

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    for(uint32_t jj = 0; jj < naxes[1]; jj++)
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            if((fabs(jj - y_center) / height) <
                    exp(-((ii - x_center) * (ii - x_center) / width / width)))
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = 1;
            }
        }

    return (ID);
}

imageID make_sectors(const char *ID_name,
                     uint32_t    l1,
                     uint32_t    l2,
                     double      x_center,
                     double      y_center,
                     double      step,
                     long        NB_sectors)
{
    imageID  ID;
    uint32_t naxes[2];
    double   theta;

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    for(uint32_t jj = 0; jj < naxes[1]; jj++)
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            theta = atan2((ii - x_center), (jj - y_center));
            if(theta < 0.0)
            {
                theta += 2.0 * PI;
            }
            data.image[ID].array.F[jj * naxes[0] + ii] =
                step * ((long)(theta / 2.0 / PI * NB_sectors));
        }

    return (ID);
}

imageID
make_rnd(const char *ID_name, uint32_t l1, uint32_t l2, const char *options)
{
    imageID  ID;
    uint32_t naxes[2];
    int      distrib;
    uint64_t nelement;

    distrib = 0; /* uniform */
    if(strstr(options, "gauss") != NULL)
    {
        distrib = 1; /* gauss */
        printf("gaussian distribution\n");
    }

    if(strstr(options, "trgauss") != NULL)
    {
        distrib = 2; /* truncated gauss */
        printf("truncated gaussian distribution\n");
    }

    if(data.Debug > 1)
    {
        fprintf(stdout, "Image size = %u %u\n", l1, l2);
    }

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];
    nelement = naxes[0] * naxes[1];

    // openMP is slow when calling gsl random number generator : do not use openMP here
    if(distrib == 0)
    {
        for(uint64_t ii = 0; ii < nelement; ii++)
        {
            data.image[ID].array.F[ii] = (double) ran1();
        }
    }
    if(distrib == 1)
    {
        for(uint64_t ii = 0; ii < nelement; ii++)
        {
            data.image[ID].array.F[ii] = (double) gauss();
        }
    }
    if(distrib == 2)
    {
        for(uint64_t ii = 0; ii < nelement; ii++)
        {
            data.image[ID].array.F[ii] = (double) gauss_trc();
        }
    }

    return (ID);
}

imageID make_rnd_double(const char *ID_name,
                        uint32_t    l1,
                        uint32_t    l2,
                        const char *options)
{
    imageID  ID;
    uint32_t naxes[2];
    int      distrib;
    uint64_t nelement;

    distrib = 0; /* uniform */
    if(strstr(options, "gauss") != NULL)
    {
        distrib = 1; /* gauss */
        printf("gaussian distribution\n");
    }

    if(strstr(options, "trgauss") != NULL)
    {
        distrib = 2; /* truncated gauss */
        printf("truncated gaussian distribution\n");
    }

    if(data.Debug > 1)
    {
        fprintf(stdout, "Image size = %u %u\n", l1, l2);
    }

    create_2Dimage_ID_double(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];
    nelement = naxes[0] * naxes[1];

    // openMP is slow when calling gsl random number generator : do not use openMP here
    if(distrib == 0)
    {
        for(uint64_t ii = 0; ii < nelement; ii++)
        {
            data.image[ID].array.D[ii] = (double) ran1();
        }
    }
    if(distrib == 1)
    {
        for(uint64_t ii = 0; ii < nelement; ii++)
        {
            data.image[ID].array.D[ii] = (double) gauss();
        }
    }
    if(distrib == 2)
    {
        for(uint64_t ii = 0; ii < nelement; ii++)
        {
            data.image[ID].array.D[ii] = (double) gauss_trc();
        }
    }

    return (ID);
}

/*
int make_rnd1(const char *ID_name, long l1, long l2, const char *options)
{
  int ID;
  long naxes[2];
  int distrib;
  long nelements;
  struct prng *g;

  distrib = 0;
  if (strstr(options,"-gauss")!=NULL)
    {
      distrib = 1;
    }

  if (strstr(options,"-trgauss")!=NULL)
    {
      distrib = 2;
      printf("truncated gaussian distribution\n");
   }

  g = prng_new("eicg(2147483647,111,1,0)");

   if (g == NULL)
   {
      fprintf(stderr,"Initialisation of generator failed.\n");
      exit (-1);
   }

   printf("Short name: %s\n",prng_short_name(g));

   printf("Expanded name: %s\n",prng_long_name(g));


   create_2Dimage_ID(ID_name,l1,l2);
   ID = image_ID(ID_name);
   naxes[0] = data.image[ID].md[0].size[0];
   naxes[1] = data.image[ID].md[0].size[1];
   nelements=naxes[0]*naxes[1];

   prng_get_array(g,data.image[ID].array.F,nelements);
   prng_reset(g);
   prng_free(g);


   return(0);
}
*/

imageID
make_gauss(const char *ID_name, uint32_t l1, uint32_t l2, double a, double A)
{
    imageID  ID;
    uint32_t naxes[2];
    double   distsq;

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    for(uint32_t jj = 0; jj < naxes[1]; jj++)
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            distsq = (ii - naxes[0] / 2) * (ii - naxes[0] / 2) +
                     (jj - naxes[1] / 2) * (jj - naxes[1] / 2);
            data.image[ID].array.F[jj * naxes[0] + ii] =
                (double) A * exp(-distsq / a / a);
        }
    /*  printf("FWHM = %f\n",2.0*a*sqrt(log(2)));*/
    return (ID);
}

imageID make_FiberCouplingOverlap(const char *ID_name)
{
    imageID  ID;
    uint32_t naxes[2];
    uint32_t size = 128;

    create_2Dimage_ID(ID_name, size, size, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    float TTcoeff = 0.2;

    float puprad = 0.1 * size;
    float xcent  = 1.32;
    float ycent  = 0.0;

    // compute TEM00 map
    imageID IDtem00;
    create_2Dimage_ID("tem00", size, size, &IDtem00);

    double fluxtot = 0.0;
    for(uint32_t jj = 0; jj < naxes[1]; jj++)
    {
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            float x     = 1.0 * (1.0 * ii - 0.5 * naxes[0]) / puprad;
            float y     = 1.0 * (1.0 * jj - 0.5 * naxes[1]) / puprad;
            float r0    = sqrt(x * x + y * y);
            float TEM00 = exp(-r0 * r0);

            fluxtot += TEM00 * TEM00;
            data.image[IDtem00].array.F[jj * naxes[0] + ii] = TEM00;
        }
    }

    for(uint32_t jj = 0; jj < naxes[1]; jj++)
    {
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            data.image[IDtem00].array.F[jj * naxes[0] + ii] /= sqrt(fluxtot);
        }
    }

    for(uint32_t jj = 0; jj < naxes[1]; jj++)
    {
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            double totre = 0.0;
            double totim = 0.0;

            float TTx = 1.0 * (1.0 * ii - 0.5 * naxes[0]) * TTcoeff;
            float TTy = 1.0 * (1.0 * jj - 0.5 * naxes[1]) * TTcoeff;

            fluxtot = 0.0;
            for(uint32_t jj0 = 0; jj0 < naxes[1]; jj0++)
            {
                for(uint32_t ii0 = 0; ii0 < naxes[0]; ii0++)
                {
                    float pup_ampl;
                    float pup_pha;

                    // pupil coord x, y

                    float x  = 1.0 * (1.0 * ii0 - 0.5 * naxes[0]) / puprad;
                    float y  = 1.0 * (1.0 * jj0 - 0.5 * naxes[1]) / puprad;
                    float dx = x - xcent;
                    float dy = y - ycent;

                    float r = sqrt(dx * dx + dy * dy);

                    float TEM00 =
                        data.image[IDtem00].array.F[jj0 * naxes[0] + ii0];

                    //data.image[ID].array.F[jj * naxes[0] + ii] = -r;

                    if((r < 1.0) && (r > 0.3))
                    {
                        pup_ampl =
                            1.0; //data.image[IDtem00].array.F[jj0 * naxes[0] + ii0];
                        pup_pha = x * TTx + y * TTy;

                        fluxtot += pup_ampl * pup_ampl;

                        totre += TEM00 * (pup_ampl * cos(pup_pha));
                        totim += TEM00 * (pup_ampl * sin(pup_pha));
                    }
                    else
                    {
                        data.image[ID].array.F[jj * naxes[0] + ii] = 0.0;
                        pup_ampl                                   = 0.0;
                    }
                }
            }

            data.image[ID].array.F[jj * naxes[0] + ii] =
                (totre * totre + totim * totim) / sqrt(fluxtot);
        }
    }

    return ID;
}

imageID make_2axis_gauss(const char *ID_name,
                         uint32_t    l1,
                         uint32_t    l2,
                         double      a,
                         double      A,
                         double      E,
                         double      PA)
{
    imageID  ID;
    uint32_t naxes[2];
    double   distsq;
    double   iin, jjn;

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    for(uint32_t jj = 0; jj < naxes[1]; jj++)
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            iin = 1.0 * (ii - naxes[0] / 2) * cos(PA) +
                  1.0 * (jj - naxes[1] / 2) * sin(PA);
            jjn = 1.0 * (jj - naxes[1] / 2) * cos(PA) -
                  1.0 * (ii - naxes[0] / 2) * sin(PA);
            distsq = iin * iin + (1.0 / (1.0 + E)) * jjn * jjn;
            data.image[ID].array.F[jj * naxes[0] + ii] =
                (double) A * exp(-distsq / a / a);
        }

    return (ID);
}

imageID
make_cluster(const char *ID_name, uint32_t l1, uint32_t l2, const char *options)
{
    imageID  ID;
    uint32_t naxes[2];
    long     nb_star       = 3000;
    double   cluster_size  = 0.1; /* relative to the FOV */
    double   concentration = 1.0;
    long     i;
    double   tmp, dist, angle;
    char     input[50];
    int      str_pos;
    int      sim = 0;
    long     lii, ljj, hii, hjj;

    if(strstr(options, "-nbstars ") != NULL)
    {
        str_pos = strstr(options, "-nbstars ") - options;
        str_pos = str_pos + strlen("-nbstars ");
        i       = 0;
        while((options[i + str_pos] != ' ') &&
                (options[i + str_pos] != '\n') && (options[i + str_pos] != '\0'))
        {
            input[i] = options[i + str_pos];
            i++;
        }
        input[i] = '\0';
        nb_star  = atol(input);
        printf("number of stars is %ld\n", nb_star);
    }

    if(strstr(options, "-conc ") != NULL)
    {
        str_pos = strstr(options, "-conc ") - options;
        str_pos = str_pos + strlen("-conc ");
        i       = 0;
        while((options[i + str_pos] != ' ') &&
                (options[i + str_pos] != '\n') && (options[i + str_pos] != '\0'))
        {
            input[i] = options[i + str_pos];
            i++;
        }
        input[i]      = '\0';
        concentration = atof(input);
        printf("concentration is %f\n", concentration);
    }

    if(strstr(options, "-size ") != NULL)
    {
        str_pos = strstr(options, "-size ") - options;
        str_pos = str_pos + strlen("-size ");
        i       = 0;
        while((options[i + str_pos] != ' ') &&
                (options[i + str_pos] != '\n') && (options[i + str_pos] != '\0'))
        {
            input[i] = options[i + str_pos];
            i++;
        }
        input[i]     = '\0';
        cluster_size = atof(input);
        printf("cluster size is %f\n", cluster_size);
    }

    if(strstr(options, "-sim") != NULL)
    {
        printf("all sources in the central half array \n");
        sim = 1;
    }

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    if(sim == 0)
    {
        lii = 0;
        ljj = 0;
        hii = naxes[0];
        hjj = naxes[1];
    }
    else
    {
        lii = naxes[0] / 4;
        ljj = naxes[1] / 4;
        hii = 3 * naxes[0] / 4;
        hjj = 3 * naxes[1] / 4;
    }

    i = 0;
    while(i < nb_star)
    {
        dist        = gauss();
        dist        = sqrt(sqrt(dist * dist));
        dist        = pow(dist, concentration);
        angle       = 2 * PI * ran1();
        uint32_t ii = (uint32_t)(naxes[0] / 2 + (cluster_size * naxes[0] / 2) *
                                 dist * cos(angle));
        uint32_t jj = (uint32_t)(naxes[1] / 2 + (cluster_size * naxes[1] / 2) *
                                 dist * sin(angle));

        if((ii > lii) && (jj > ljj) && (ii < hii) && (jj < hjj))
        {
            tmp = gauss();
            data.image[ID].array.F[jj * naxes[0] + ii] += tmp * tmp;
            i++;
        }
    }

    return (ID);
}

imageID make_galaxy(const char *ID_name,
                    uint32_t    l1,
                    uint32_t    l2,
                    double      S_radius,
                    double      S_L0,
                    double      S_ell,
                    double      S_PA,
                    double      E_radius,
                    double      E_L0,
                    double      E_ell,
                    double      E_PA)
{
    imageID  ID;
    uint32_t naxes[2];
    double   x, y, r;
    double   aob, boa; /* a over b and b over a */
    double   total = 0;

    /* E = 1-b/a */

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = l1;
    naxes[1] = l2;

    /* Spiral component */
    aob = 1.0 / (1.0 - S_ell);
    boa = 1.0 - S_ell;

    for(uint32_t ii = 0; ii < naxes[0]; ii++)
        for(uint32_t jj = 0; jj < naxes[1] / 2 + 1; jj++)
        {
            x = cos(S_PA) * (ii - naxes[0] / 2) +
                sin(S_PA) * (jj - naxes[1] / 2);
            y = -sin(S_PA) * (ii - naxes[0] / 2) +
                cos(S_PA) * (jj - naxes[1] / 2);
            r = sqrt(aob * x * x + boa * y * y);
            data.image[ID].array.F[jj * naxes[0] + ii] =
                S_L0 * exp(-r / S_radius);
        }

    /* Elliptical component */
    aob = 1.0 / (1.0 - E_ell);
    boa = 1.0 - E_ell;

    for(uint32_t ii = 0; ii < naxes[0]; ii++)
        for(uint32_t jj = 0; jj < naxes[1] / 2 + 1; jj++)
        {
            x = cos(E_PA) * (ii - naxes[0] / 2) +
                sin(E_PA) * (jj - naxes[1] / 2);
            y = -sin(E_PA) * (ii - naxes[0] / 2) +
                cos(E_PA) * (jj - naxes[1] / 2);
            r = sqrt(aob * x * x + boa * y * y);
            data.image[ID].array.F[jj * naxes[0] + ii] +=
                E_L0 * pow(10.0, (-3.3307 * (pow((r / E_radius), 0.25) - 1.0)));
        }

    /* filling other half */
    for(uint32_t ii = 1; ii < naxes[0]; ii++)
        for(uint32_t jj = 1; jj < naxes[1] / 2; jj++)
        {
            data.image[ID]
            .array.F[(naxes[1] - jj) * naxes[0] + (naxes[0] - ii)] =
                data.image[ID].array.F[jj * naxes[0] + ii];
        }
    uint32_t ii = 0;
    for(uint32_t jj = naxes[1] / 2; jj < naxes[1]; jj++)
    {
        aob = 1.0 / (1.0 - S_ell);
        boa = 1.0 - S_ell;
        x   = cos(S_PA) * (ii - naxes[0] / 2) + sin(S_PA) * (jj - naxes[1] / 2);
        y = -sin(S_PA) * (ii - naxes[0] / 2) + cos(S_PA) * (jj - naxes[1] / 2);
        r = sqrt(aob * x * x + boa * y * y);
        data.image[ID].array.F[jj * naxes[0] + ii] = S_L0 * exp(-r / S_radius);
        aob                                        = 1.0 / (1.0 - E_ell);
        boa                                        = 1.0 - E_ell;
        x = cos(E_PA) * (ii - naxes[0] / 2) + sin(E_PA) * (jj - naxes[1] / 2);
        y = -sin(E_PA) * (ii - naxes[0] / 2) + cos(E_PA) * (jj - naxes[1] / 2);
        r = sqrt(aob * x * x + boa * y * y);
        data.image[ID].array.F[jj * naxes[0] + ii] +=
            E_L0 * pow(10.0, (-3.3307 * (pow((r / E_radius), 0.25) - 1.0)));
    }

    total = 2.0 * PI * S_L0 * S_radius * S_radius +
            23.02 * E_L0 * E_radius * E_radius;
    printf("total : %f (%f)\n", arith_image_total(ID_name), total);

    return (ID);
}

imageID
make_Egalaxy(const char *ID_name, uint32_t l1, uint32_t l2, const char *options)
{
    imageID  ID;
    uint32_t naxes[2];
    double   galaxy_size   = 0.1; /* relative to the FOV */
    double   concentration = 1.0;
    long     i;
    double   PA   = 0;
    double   E    = 0.3; /* position angle and ellipticity */
    double   peak = 1;   /* maximum value */
    char     input[50];
    int      str_pos;
    int      sim = 0;
    long     lii, ljj, hii, hjj;
    double   x, y, xcenter, ycenter, distsq;

    if(strstr(options, "-conc ") != NULL)
    {
        str_pos = strstr(options, "-conc ") - options;
        str_pos = str_pos + strlen("-conc ");
        i       = 0;
        while((options[i + str_pos] != ' ') &&
                (options[i + str_pos] != '\n') && (options[i + str_pos] != '\0'))
        {
            input[i] = options[i + str_pos];
            i++;
        }
        input[i]      = '\0';
        concentration = atof(input);
        printf("concentration is %f\n", concentration);
    }

    if(strstr(options, "-size ") != NULL)
    {
        str_pos = strstr(options, "-size ") - options;
        str_pos = str_pos + strlen("-size ");
        i       = 0;
        while((options[i + str_pos] != ' ') &&
                (options[i + str_pos] != '\n') && (options[i + str_pos] != '\0'))
        {
            input[i] = options[i + str_pos];
            i++;
        }
        input[i]    = '\0';
        galaxy_size = atof(input);
        printf("size is %f\n", galaxy_size);
    }

    if(strstr(options, "-pa ") != NULL)
    {
        str_pos = strstr(options, "-pa ") - options;
        str_pos = str_pos + strlen("-pa ");
        i       = 0;
        while((options[i + str_pos] != ' ') &&
                (options[i + str_pos] != '\n') && (options[i + str_pos] != '\0'))
        {
            input[i] = options[i + str_pos];
            i++;
        }
        input[i] = '\0';
        PA       = atof(input);
        printf("galaxy pa size is %f radians \n", PA);
    }

    if(strstr(options, "-e ") != NULL)
    {
        str_pos = strstr(options, "-e ") - options;
        str_pos = str_pos + strlen("-e ");
        i       = 0;
        while((options[i + str_pos] != ' ') &&
                (options[i + str_pos] != '\n') && (options[i + str_pos] != '\0'))
        {
            input[i] = options[i + str_pos];
            i++;
        }
        input[i] = '\0';
        E        = atof(input);
        printf("galaxy elipticity is %f \n", E);
    }

    if(strstr(options, "-sim") != NULL)
    {
        printf("all sources in the central half array \n");
        sim = 1;
    }

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];
    xcenter  = naxes[0] / 2;
    ycenter  = naxes[1] / 2;

    if(sim == 0)
    {
        lii = 0;
        ljj = 0;
        hii = naxes[0];
        hjj = naxes[1];
    }
    else
    {
        lii = naxes[0] / 4;
        ljj = naxes[1] / 4;
        hii = 3 * naxes[0] / 4;
        hjj = 3 * naxes[1] / 4;
    }

    for(uint32_t jj = ljj; jj < hjj; jj++)
        for(uint32_t ii = lii; ii < hii; ii++)
        {
            x = cos(PA) * (ii - xcenter) + sin(PA) * (jj - ycenter);
            y = -sin(PA) * (ii - xcenter) + cos(PA) * (jj - ycenter);
            /* E = sqrt(a*a-b*b)/a */
            /* a = 1 */
            x      = x;
            y      = y / sqrt(1 - E * E);
            distsq = (x * x + y * y) /
                     (naxes[0] * naxes[0] + naxes[1] * naxes[1]) / galaxy_size /
                     galaxy_size;
            data.image[ID].array.F[jj * naxes[0] + ii] =
                (double) peak * exp(-concentration * distsq);
        }

    return (ID);
}

// for sol system, index ~2.4 with local zodi
imageID gen_image_EZdisk(const char *ID_name,
                         uint32_t    size,
                         double      InnerEdge,
                         double      Index,
                         double      Incl)
{
    imageID ID;
    double  x, y, r, r0;
    double  value;

    create_2Dimage_ID(ID_name, size, size, &ID);
    r0 = 6.0;
    for(uint32_t ii = 0; ii < size; ii++)
        for(uint32_t jj = 0; jj < size; jj++)
        {
            x = 1.0 * (ii + 0.5) - size / 2;
            y = 1.0 * (jj + 0.5) - size / 2;
            y /= cos(Incl);
            r = sqrt(x * x + y * y);
            if(r < InnerEdge)
            {
                value = 0.0;
            }
            else
            {
                value = pow(r, -Index);
            }
            value /= cos(Incl);

            value += pow(r0, -Index);
            data.image[ID].array.F[jj * size + ii] = value;
        }

    return (ID);
}

imageID make_slopexy(
    const char *ID_name, uint32_t l1, uint32_t l2, double sx, double sy)
{
    imageID  ID;
    uint32_t naxes[2];
    double   coeff;

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    coeff = sx * (naxes[0] / 2) + sy * (naxes[1] / 2);

    for(uint32_t jj = 0; jj < naxes[1]; jj++)
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            data.image[ID].array.F[jj * naxes[0] + ii] =
                sx * ii + sy * jj - coeff;
        }

    return (ID);
}

imageID
make_dist(const char *ID_name, uint32_t l1, uint32_t l2, double f1, double f2)
{
    imageID  ID;
    uint32_t naxes[2];

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    for(uint32_t jj = 0; jj < naxes[1]; jj++)
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            data.image[ID].array.F[jj * naxes[0] + ii] =
                sqrt((f1 - ii) * (f1 - ii) + (f2 - jj) * (f2 - jj));
        }

    return (ID);
}

imageID make_PosAngle(
    const char *ID_name, uint32_t l1, uint32_t l2, double f1, double f2)
{
    imageID  ID;
    uint32_t naxes[2];

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    for(uint32_t jj = 0; jj < naxes[1]; jj++)
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            double x, y;
            x                                          = 1.0 * ii - f1;
            y                                          = 1.0 * jj - f2;
            data.image[ID].array.F[jj * naxes[0] + ii] = atan2(y, x);
        }

    return (ID);
}

imageID make_psf_from_profile(const char *profile_name,
                              const char *ID_name,
                              uint32_t    l1,
                              uint32_t    l2)
{
    imageID  ID;
    uint32_t naxes[2];
    FILE    *fp;
    long     nb_lines;
    char     lstring[1000];
    char     line[200];
    double  *distarr;
    double  *valarr;
    long     i;
    double   dist;
    float    fl1, fl2;

    create_2Dimage_ID(ID_name, l1, l2, &ID);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];

    /* compute number of lines */
    sprintf(lstring, "wc -l %s > tmpcnt.txt", profile_name);
    if(system(lstring) == -1)
    {
        printf("ERROR: system(\"%s\"), %s line %d\n",
               lstring,
               __FILE__,
               __LINE__);
        exit(0);
    }
    if((fp = fopen("tmpcnt.txt", "r")) == NULL)
    {
        printf("error : can't open file \"tmpcnt.txt\"\n");
    }
    if(fgets(line, 200, fp) == NULL)
    {
        printf("ERROR: fgets, %s line %d\n", __FILE__, __LINE__);
        exit(0);
    }
    fclose(fp);
    printf("%s\n", line);
    fflush(stdout);
    sscanf(line, "%ld %s", &nb_lines, lstring);

    printf("%ld lines\n", nb_lines);

    distarr = (double *) malloc(sizeof(double) * nb_lines);
    if(distarr == NULL)
    {
        PRINT_ERROR("malloc returns NULL pointer");
        abort();
    }

    valarr = (double *) malloc(sizeof(double) * nb_lines);
    if(valarr == NULL)
    {
        PRINT_ERROR("malloc returns NULL pointer");
        abort();
    }

    if((fp = fopen(profile_name, "r")) == NULL)
    {
        printf("error : can't open file \"%s\"\n", profile_name);
    }

    for(i = 0; i < nb_lines; i++)
    {
        if(fgets(line, 200, fp) == NULL)
        {
            printf("ERROR: fgets, %s line %d\n", __FILE__, __LINE__);
            exit(0);
        }
        sscanf(line, "%f %f", &fl1, &fl2);
        distarr[i] = fl1;
        valarr[i]  = fl2;
    }
    fclose(fp);

    for(uint32_t jj = 0; jj < naxes[1]; jj++)
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
        {
            dist = sqrt((ii - naxes[0] / 2) * (ii - naxes[0] / 2) +
                        (jj - naxes[1] / 2) * (jj - naxes[1] / 2));
            i    = 0;
            while((distarr[i] < dist) && (i != nb_lines - 1))
            {
                i++;
            }
            if(i != 0)
            {
                data.image[ID].array.F[jj * naxes[0] + ii] =
                    valarr[i - 1] + (valarr[i] - valarr[i - 1]) *
                    (dist - distarr[i - 1]) /
                    (distarr[i] - distarr[i - 1]);
            }
            else
            {
                data.image[ID].array.F[jj * naxes[0] + ii] = valarr[0];
            }
        }

    free(distarr);
    free(valarr);

    return (ID);
}

imageID make_offsetHyperGaussian(
    uint32_t size, double a, double b, long n, const char *IDname)
{
    imageID ID;

    create_2Dimage_ID(IDname, size, size, &ID);
    for(uint32_t ii = 0; ii < size; ii++)
        for(uint32_t jj = 0; jj < size; jj++)
        {
            double x, y, dist;

            x    = 1.0 * ii - size / 2;
            y    = 1.0 * jj - size / 2;
            dist = sqrt(x * x + y * y);
            if(dist < a)
            {
                data.image[ID].array.F[jj * size + ii] = 0.0;
            }
            else
            {
                data.image[ID].array.F[jj * size + ii] =
                    1.0 - exp(-pow((dist - a) / b, n));
            }
        }

    return (ID);
}

imageID
make_cosapoedgePupil(uint32_t size, double a, double b, const char *IDname)
{
    imageID ID;

    create_2Dimage_ID(IDname, size, size, &ID);
    for(uint32_t ii = 0; ii < size; ii++)
        for(uint32_t jj = 0; jj < size; jj++)
        {
            double x, y, dist;

            x    = 1.0 * ii - size / 2;
            y    = 1.0 * jj - size / 2;
            dist = sqrt(x * x + y * y);
            if(dist < a)
            {
                data.image[ID].array.F[jj * size + ii] = 1.0;
            }
            else if(dist > b)
            {
                data.image[ID].array.F[jj * size + ii] = 0.0;
            }
            else
            {
                data.image[ID].array.F[jj * size + ii] =
                    0.5 * (cos(PI * (dist - a) / (b - a)) + 1.0);
            }
        }

    return ID;
}

// make square grid of pixels
imageID make_2Dgridpix(const char *IDname,
                       uint32_t    xsize,
                       uint32_t    ysize,
                       double      pitchx,
                       double      pitchy,
                       double      offsetx,
                       double      offsety)
{
    imageID ID;
    double  x, y;
    long    i, j;
    double  u, t;

    create_2Dimage_ID(IDname, xsize, ysize, &ID);
    for(x = offsetx; x < xsize - 1; x += pitchx)
        for(y = offsety; y < ysize - 1; y += pitchy)
        {
            i                                           = (long) x;
            j                                           = (long) y;
            u                                           = x - i;
            t                                           = y - j;
            data.image[ID].array.F[j * xsize + i]       = (1.0 - u) * (1.0 - t);
            data.image[ID].array.F[(j + 1) * xsize + i] = (1.0 - u) * t;
            data.image[ID].array.F[j * xsize + i + 1]   = u * (1.0 - t);
            data.image[ID].array.F[(j + 1) * xsize + i + 1] = u * t;
        }

    return (ID);
}

// make tile
imageID make_tile(const char *IDin_name, uint32_t size, const char *IDout_name)
{
    uint32_t sizex0, sizey0; // input
    imageID  IDin, IDout;

    create_2Dimage_ID(IDout_name, size, size, &IDout);
    IDin   = image_ID(IDin_name);
    sizex0 = data.image[IDin].md[0].size[0];
    sizey0 = data.image[IDin].md[0].size[1];

    for(uint32_t ii = 0; ii < size; ii++)
        for(uint32_t jj = 0; jj < size; jj++)
        {
            uint32_t ii0 = ii % sizex0;
            uint32_t jj0 = jj % sizey0;
            data.image[IDout].array.F[jj * size + ii] =
                data.image[IDin].array.F[jj0 * sizex0 + ii0];
        }

    return (IDout);
}

// make image that is coordinate of input
// for example, if axis = 0
// value = 1.0 x ii
// if axis value is not one of the coordinates, write pixel index
//
imageID
image_gen_im2coord(const char *IDin_name, uint8_t axis, const char *IDout_name)
{
    uint8_t  naxis;
    int      OK = 1;
    imageID  IDin;
    imageID  IDout = -1;
    uint32_t xsize, ysize, zsize;

    IDin  = image_ID(IDin_name);
    naxis = data.image[IDin].md[0].naxis;

    if(axis > naxis - 1)
    {
        printf("Image has only %u axis, cannot access axis %u\n", naxis, axis);
        OK = 0;
    }

    if(naxis > 3)
    {
        printf("naxis should be 3 or less\n");
        OK = 0;
    }

    if(OK == 1)
    {

        if(naxis == 1)
        {
            printf("naxis = 1\n");
            fflush(stdout);
            xsize = data.image[IDin].md[0].size[0];
            create_1Dimage_ID(IDout_name, xsize, &IDout);
            for(uint32_t ii = 0; ii < xsize; ii++)
            {
                data.image[IDout].array.F[ii] = 1.0 * ii;
            }
        }

        if(naxis == 2)
        {
            printf("naxis = 2\n");
            fflush(stdout);
            xsize = data.image[IDin].md[0].size[0];
            ysize = data.image[IDin].md[0].size[1];
            create_2Dimage_ID(IDout_name, xsize, ysize, &IDout);

            switch(axis)
            {
                case 0:
                    for(uint32_t ii = 0; ii < xsize; ii++)
                        for(uint32_t jj = 0; jj < ysize; jj++)
                        {
                            data.image[IDout].array.F[jj * xsize + ii] = 1.0 * ii;
                        }
                    break;
                case 1:
                    for(uint32_t ii = 0; ii < xsize; ii++)
                        for(uint32_t jj = 0; jj < ysize; jj++)
                        {
                            data.image[IDout].array.F[jj * xsize + ii] = 1.0 * jj;
                        }
                    break;
                default:
                    for(uint32_t ii = 0; ii < xsize; ii++)
                        for(uint32_t jj = 0; jj < ysize; jj++)
                        {
                            data.image[IDout].array.F[jj * xsize + ii] =
                                1.0 * jj * xsize + ii;
                        }
            }
        }

        if(naxis == 3)
        {
            printf("naxis = 3\n");
            fflush(stdout);
            xsize = data.image[IDin].md[0].size[0];
            ysize = data.image[IDin].md[0].size[1];
            zsize = data.image[IDin].md[0].size[2];
            create_3Dimage_ID(IDout_name, xsize, ysize, zsize, &IDout);

            switch(axis)
            {
                case 0:
                    for(uint32_t ii = 0; ii < xsize; ii++)
                        for(uint32_t jj = 0; jj < ysize; jj++)
                            for(uint32_t kk = 0; kk < zsize; kk++)
                            {
                                data.image[IDout]
                                .array.F[kk * xsize * ysize + jj * xsize + ii] =
                                    1.0 * ii;
                            }
                    break;
                case 1:
                    for(uint32_t ii = 0; ii < xsize; ii++)
                        for(uint32_t jj = 0; jj < ysize; jj++)
                            for(uint32_t kk = 0; kk < zsize; kk++)
                            {
                                data.image[IDout]
                                .array.F[kk * xsize * ysize + jj * xsize + ii] =
                                    1.0 * jj;
                            }
                    break;
                case 2:
                    for(uint32_t ii = 0; ii < xsize; ii++)
                        for(uint32_t jj = 0; jj < xsize; jj++)
                            for(uint32_t kk = 0; kk < zsize; kk++)
                            {
                                data.image[IDout]
                                .array.F[kk * xsize * ysize + jj * xsize + ii] =
                                    1.0 * kk;
                            }
                    break;
                default:
                    for(uint32_t ii = 0; ii < xsize; ii++)
                        for(uint32_t jj = 0; jj < xsize; jj++)
                            for(uint32_t kk = 0; kk < zsize; kk++)
                            {
                                data.image[IDout]
                                .array.F[kk * xsize * ysize + jj * xsize + ii] =
                                    1.0 * kk * xsize * ysize + jj * xsize + ii;
                            }
            }
        }
    }

    return (IDout);
}

/**
 * Create Voronoi map
 *
 * filename is an ASCII file defining points
 *
 * First line is number of point
 *
 * Each following line is a point, with following format:
 * index x y
 *
 * (x,y) coordinates in range [0:1]
 *
 */
imageID
image_gen_make_voronoi_map(const char *filename,
                           const char *IDout_name,
                           uint32_t    xsize,
                           uint32_t    ysize,
                           float radius, // maximum radius of each Voronoi zone
                           float maxsep  // gap between Voronoi zones
                          )
{
    imageID   IDout;
    uint8_t   naxis = 2;
    uint32_t *sizearray;

    int64_t *nearest_index;
    float   *nearest_distance;
    int64_t *nextnearest_index;
    float   *nextnearest_distance;
    int     *gapim;

    FILE *fp;
    long  NBpt;
    fp = fopen(filename, "r");
    if(fp == NULL)
    {
        printf("file %s not found\n", filename);
        return 1;
    }

    {
        int fscanfcnt = fscanf(fp, "%ld", &NBpt);
        if(fscanfcnt == EOF)
        {
            if(ferror(fp))
            {
                perror("fscanf");
            }
            else
            {
                fprintf(stderr,
                        "Error: fscanf reached end of file, no matching "
                        "characters, no matching failure\n");
            }
            exit(EXIT_FAILURE);
        }
        else if(fscanfcnt != 2)
        {
            fprintf(stderr,
                    "Error: fscanf successfully matched and assigned %i input "
                    "items, 2 expected\n",
                    fscanfcnt);
            exit(EXIT_FAILURE);
        }
    }

    printf("Loading %ld points\n", NBpt);

    uint32_t *vpt_index = (uint32_t *) malloc(sizeof(uint32_t) * NBpt);
    if(vpt_index == NULL)
    {
        PRINT_ERROR("malloc returns NULL pointer");
        abort();
    }

    float *vpt_x = (float *) malloc(sizeof(float) * NBpt);
    if(vpt_x == NULL)
    {
        PRINT_ERROR("malloc returns NULL pointer");
        abort();
    }

    float *vpt_y = (float *) malloc(sizeof(float) * NBpt);
    if(vpt_y == NULL)
    {
        PRINT_ERROR("malloc returns NULL pointer");
        abort();
    }

    for(int pt = 0; pt < NBpt; pt++)
    {
        int fscanfcnt =
            fscanf(fp, "%u %f %f\n", &vpt_index[pt], &vpt_x[pt], &vpt_y[pt]);
        if(fscanfcnt == EOF)
        {
            if(ferror(fp))
            {
                perror("fscanf");
            }
            else
            {
                fprintf(stderr,
                        "Error: fscanf reached end of file, no matching "
                        "characters, no matching failure\n");
            }
            exit(EXIT_FAILURE);
        }
        else if(fscanfcnt != 3)
        {
            fprintf(stderr,
                    "Error: fscanf successfully matched and assigned %i input "
                    "items, 3 expected\n",
                    fscanfcnt);
            exit(EXIT_FAILURE);
        }
    }

    fclose(fp);

    /* TEST pattern
     *



    int ring;
    int NBring = 6;
    float* ringradius;
    int* ringNBpt;
    float *ringoffset;

    float radmin = 0.2;
    float radmax = 0.7;

    ringradius = (float*) malloc(sizeof(float)*NBring);
    ringNBpt = (int*) malloc(sizeof(int)*NBring);
    ringoffset = (float*) malloc(sizeof(float)*NBring);

    for(int ring=0; ring<NBring; ring++) {
        ringradius[ring] = radmin + (radmax-radmin)*ring/(NBring-1);
    }

    ringNBpt[0] = 18;
    ringNBpt[1] = 24;
    ringNBpt[2] = 30;
    ringNBpt[3] = 36;
    ringNBpt[4] = 40;
    ringNBpt[5] = 40;

    ringoffset[0] = 0.0;
    ringoffset[1] = 0.0;
    ringoffset[2] = 0.0;
    ringoffset[3] = 0.0;
    ringoffset[4] = 2.0*M_PI/40.0/2.0;
    ringoffset[5] = 0.0;

    int NBelem = 0;
    for(int ring=0; ring<NBring; ring++)
    	NBelem += ringNBpt[ring];

    FILE * fptest = fopen("vmap.test.txt", "w");
    fprintf(fptest, "%d", NBelem);
    int ptindex = 0;
    for(int ring=0; ring<NBring; ring++)
    {
        for(int elem=0; elem<ringNBpt[ring]; elem++)
        {
            float pha = 2.0*M_PI*elem/ringNBpt[ring] + ringoffset[ring];
            float x = ringradius[ring]*cos(pha);
            float y = ringradius[ring]*sin(pha);
            fprintf(fptest, "%5d %6.4f %6.4f\n", ptindex, 0.5 + 0.5*x, 0.5 + 0.5*y );
            ptindex ++;
        }
    }
    fclose(fptest);
    free(ringradius);
    free(ringNBpt);
    free(ringoffset);


    END TEST pattern
     *
     */

    sizearray = (uint32_t *) malloc(sizeof(uint32_t) * naxis);
    if(sizearray == NULL)
    {
        PRINT_ERROR("malloc returns NULL pointer");
        abort();
    }

    sizearray[0] = xsize;
    sizearray[1] = ysize;
    create_image_ID(IDout_name,
                    naxis,
                    sizearray,
                    _DATATYPE_INT32,
                    0,
                    0,
                    0,
                    &IDout);
    free(sizearray);

    nearest_index = (int64_t *) malloc(sizeof(int64_t) * xsize * ysize);
    if(nearest_index == NULL)
    {
        PRINT_ERROR("malloc returns NULL pointer");
        abort();
    }

    nearest_distance = (float *) malloc(sizeof(float) * xsize * ysize);
    if(nearest_distance == NULL)
    {
        PRINT_ERROR("malloc returns NULL pointer");
        abort();
    }

    nextnearest_index = (int64_t *) malloc(sizeof(int64_t) * xsize * ysize);
    if(nextnearest_index == NULL)
    {
        PRINT_ERROR("malloc returns NULL pointer");
        abort();
    }

    nextnearest_distance = (float *) malloc(sizeof(float) * xsize * ysize);
    if(nextnearest_distance == NULL)
    {
        PRINT_ERROR("malloc returns NULL pointer");
        abort();
    }

    gapim = (int *) malloc(sizeof(int) * xsize * ysize);
    if(gapim == NULL)
    {
        PRINT_ERROR("malloc returns NULL pointer");
        abort();
    }

    // initialize arrays
    float bigval = 1.0e20;
    for(uint64_t ii = 0; ii < xsize * ysize; ii++)
    {
        nearest_index[ii]                = -1;
        nearest_distance[ii]             = bigval;
        nextnearest_index[ii]            = -1;
        nextnearest_distance[ii]         = bigval;
        data.image[IDout].array.SI32[ii] = -1;
    }

    for(uint32_t ii = 0; ii < xsize; ii++)
        for(uint32_t jj = 0; jj < ysize; jj++)
        {
            int   pindex = jj * xsize + ii;
            float x      = 1.0 * ii / xsize;
            float y      = 1.0 * jj / ysize;

            for(int pt = 0; pt < NBpt; pt++)
            {
                float dx = x - vpt_x[pt];
                float dy = y - vpt_y[pt];

                float dist = sqrt(dx * dx + dy * dy);

                if(dist < nearest_distance[pindex])
                {
                    nextnearest_index[pindex]    = nearest_index[pindex];
                    nextnearest_distance[pindex] = nearest_distance[pindex];

                    nearest_index[pindex]    = pt;
                    nearest_distance[pindex] = dist;
                }
                else if(dist < nextnearest_distance[pindex])
                {
                    nextnearest_index[pindex]    = pt;
                    nextnearest_distance[pindex] = dist;
                }
            }
            if((nearest_distance[pindex] < radius))
            {
                data.image[IDout].array.SI32[pindex] = nearest_index[pindex];
            }
        }

    // add gap
    int gapsizepix = (int)(maxsep * xsize);
    // int gapsizepix2 = (int) (maxsep*xsize/sqrt(2.0));

    for(uint32_t ii = 0; ii < xsize; ii++)
        for(uint32_t jj = 0; jj < ysize; jj++)
        {
            gapim[jj * xsize + ii] = 0;
        }

    for(uint32_t ii = gapsizepix; ii < xsize - gapsizepix; ii++)
        for(uint32_t jj = gapsizepix; jj < ysize - gapsizepix; jj++)
        {
            int pindex0  = jj * xsize + ii;
            int pindex0p = jj * xsize + ii + gapsizepix;
            int pindex0m = jj * xsize + ii - gapsizepix;
            int pindexp0 = (jj + gapsizepix) * xsize + ii;
            int pindexm0 = (jj - gapsizepix) * xsize + ii;
            int pindexpp = (jj + gapsizepix) * xsize + ii + gapsizepix;
            int pindexpm = (jj + gapsizepix) * xsize + ii - gapsizepix;
            int pindexmp = (jj - gapsizepix) * xsize + ii + gapsizepix;
            int pindexmm = (jj - gapsizepix) * xsize + ii - gapsizepix;

            int32_t pv0p = data.image[IDout].array.SI32[pindex0p];
            int32_t pv0m = data.image[IDout].array.SI32[pindex0m];
            int32_t pvp0 = data.image[IDout].array.SI32[pindexp0];
            int32_t pvm0 = data.image[IDout].array.SI32[pindexm0];
            int32_t pvpp = data.image[IDout].array.SI32[pindexpp];
            int32_t pvpm = data.image[IDout].array.SI32[pindexpm];
            int32_t pvmp = data.image[IDout].array.SI32[pindexmp];
            int32_t pvmm = data.image[IDout].array.SI32[pindexmm];

            gapim[pindex0] = 1;

            if((pv0p != pv0m) || (pvp0 != pvm0) || (pvpp != pvmm) ||
                    (pvpm != pvmp))
            {
                gapim[pindex0] = 0;
            }
        }

    for(uint32_t ii = 0; ii < xsize; ii++)
        for(uint32_t jj = 0; jj < ysize; jj++)
        {
            int pindex = jj * xsize + ii;
            if(gapim[pindex] == 0)
            {
                data.image[IDout].array.SI32[pindex] = -1;
            }
        }

    free(vpt_index);
    free(vpt_x);
    free(vpt_y);

    free(nearest_index);
    free(nearest_distance);
    free(nextnearest_index);
    free(nextnearest_distance);

    free(gapim);

    return (IDout);
}
