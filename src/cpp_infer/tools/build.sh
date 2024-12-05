# OPENCV_DIR=your_opencv_dir
# LIB_DIR=your_paddle_inference_dir
OPENCV_DIR=/home/wfy/Documents/OCR/PaddleOCR/opencv3                   #根据自己路径修改
LIB_DIR=/home/wfy/Documents/OCR/PaddleOCR/paddle_inference           #根据自己路径修改
# CUDA_LIB_DIR=/usr/local/cuda-11.1/lib64
# CUDNN_LIB_DIR=your_cudnn_lib_dir

BUILD_DIR=build
rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}
cd ${BUILD_DIR}
cmake .. \
    -DPADDLE_LIB=${LIB_DIR} \
    -DWITH_MKL=ON \
    -DWITH_GPU=OFF \
    -DWITH_STATIC_LIB=OFF \
    -DWITH_TENSORRT=OFF \
    -DOPENCV_DIR=${OPENCV_DIR} \
    # -DCUDNN_LIB=${CUDNN_LIB_DIR} \
    # -DCUDA_LIB=${CUDA_LIB_DIR} \
    # -DTENSORRT_DIR=${TENSORRT_DIR} \

make -j24
