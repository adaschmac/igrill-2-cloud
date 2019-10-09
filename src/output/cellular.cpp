/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * cellular.cpp - handle connecting over cell and uploading the data to the
 *                specified server. This does *not* use the particle cloud,
 *                but instead uploads to a UDP Graphite endpoint. It's also
 *                not meant to be battery efficient.
 *
 */

#include "igrill-config.h"
#include "output/cellular.h"

