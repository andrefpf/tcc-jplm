JPLM_BINS="bin"  
INPUT_JPL_FILE="RAW/encoded/bikes.jpl"
PGX_PATH="RAW/decoded/pgx/"
DECODED_PATH="RAW/decoded/Bikes/"

rm -rf ${PGX_PATH} ${DECODED_PATH}
mkdir -p ${PGX_PATH} ${DECODED_PATH}

${JPLM_BINS}/jpl-decoder-bin --show-progress-bar --show-runtime-statistics \
    --input ${INPUT_JPL_FILE} --output ${PGX_PATH} 

for i in ${PGX_PATH}/0/*;  
do  
  filename=$(echo "$(basename $i .pgx).ppm")
  ${JPLM_BINS}/utils/convert_pgx_to_ppm --input ${PGX_PATH} --output ${DECODED_PATH}/$filename
done