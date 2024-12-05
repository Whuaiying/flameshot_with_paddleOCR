sudo rm -rf *
sudo cmake ..
sudo make -j24
sudo make install
sudo cp -r ../inference src/
sudo cp ../ppocr_keys_v1.txt src/

