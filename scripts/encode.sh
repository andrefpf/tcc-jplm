JPLM_BINS="bin"  
INPUT_PATH="RAW/datasets/Bikes/"
SHIFTED_PATH="RAW/encoded/shifted/"
PGX_PATH="RAW/encoded/pgx/"
OUTPUT_JPL_FILE="RAW/encoded/bikes.jpl"

# Create new tmp folders
rm -rf ${SHIFTED_PATH} ${PGX_PATH}
mkdir -p ${SHIFTED_PATH} ${PGX_PATH}

# Adjust inputs to a valid format name with homogeneous luminance
${JPLM_BINS}/utils/lenslet_13x13_shifter -i ${INPUT_PATH} -o ${SHIFTED_PATH} encoder

# Convert PPM files to PGX
for i in ${SHIFTED_PATH}/*;  
do  
  ${JPLM_BINS}/utils/convert_ppm_to_pgx -i $i -o ${PGX_PATH};
done

# Run JPLM
${JPLM_BINS}/jpl-encoder-bin --show-progress-bar --show-runtime-statistics --part 2  \
    --type 0 --enum-cs YCbCr_2 -u 625 -v 434 -t 13 -s 13 -nc 3 --show-error-estimate \
    --border_policy 1 --lambda 10000 --transform_size_maximum_inter_view_vertical 13 \
    --transform_size_maximum_inter_view_horizontal 13                                \
    --transform_size_maximum_intra_view_vertical 31                                  \
    --transform_size_maximum_intra_view_horizontal 31                                \
    --transform_size_minimum_inter_view_vertical 13                                  \
    --transform_size_minimum_inter_view_horizontal 13                                \
    --transform_size_minimum_intra_view_vertical 4                                   \
    --transform_size_minimum_intra_view_horizontal 4                                 \
    --input ${PGX_PATH} --output ${OUTPUT_JPL_FILE}
