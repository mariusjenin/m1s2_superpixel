#!/bin/bash

source values_bash.sh

###########################################################################################################
############################################# SLIC ########################################################
###########################################################################################################
cd build;
make -j;

#CHAMELEON
for m in "${n_mode[@]}"
do
  for i in "${n_sp_chameleon[@]}"
  do
    for j in "${n_compactness[@]}"
    do
      ./m1s2_superpixel "../img/chameleon.pgm" "../img_result/chameleon/pgm/${m}_${i}_${j}.pgm" $m $i $j;
      ./m1s2_superpixel "../img/chameleon.ppm" "../img_result/chameleon/ppm/${m}_${i}_${j}.ppm" $m $i $j;
    done
  done
done

#ALCAZAR LIL
for m in "${n_mode[@]}"
do
  for i in "${n_sp_alcazar[@]}"
  do
    for j in "${n_compactness[@]}"
    do
      ./m1s2_superpixel "../img/alcazar_lil.pgm" "../img_result/alcazar/pgm/${m}_${i}_${j}.pgm" $m $i $j;
      ./m1s2_superpixel "../img/alcazar_lil.ppm" "../img_result/alcazar/ppm/${m}_${i}_${j}.ppm" $m $i $j;
    done
  done
done

#JUNGLE LIL
for m in "${n_mode[@]}"
do
  for i in "${n_sp_jungle[@]}"
  do
    for j in "${n_compactness[@]}"
    do
      ./m1s2_superpixel "../img/jungle_lil.pgm" "../img_result/jungle/pgm/${m}_${i}_${j}.pgm" $m $i $j;
      ./m1s2_superpixel "../img/jungle_lil.ppm" "../img_result/jungle/ppm/${m}_${i}_${j}.ppm" $m $i $j;
    done
  done
done

cd ..;

###########################################################################################################
############################################# RLE #########################################################
###########################################################################################################
g++ rle.cpp;

CHAMELEON
for m in "${n_mode[@]}"
do
  for i in "${n_sp_chameleon[@]}"
  do
    for j in "${n_compactness[@]}"
    do
      ./a.out "img_result/chameleon/pgm/${m}_${i}_${j}.pgm" "img_result/chameleon/rle/pgm/${m}_${i}_${j}.rle";
      ./a.out "img_result/chameleon/ppm/${m}_${i}_${j}.ppm" "img_result/chameleon/rle/ppm/${m}_${i}_${j}.rle";
    done
  done
done


#ALCAZAR LIL
for m in "${n_mode[@]}"
do
  for i in "${n_sp_alcazar[@]}"
  do
    for j in "${n_compactness[@]}"
    do
      ./a.out "img_result/alcazar/pgm/${m}_${i}_${j}.pgm" "img_result/alcazar/rle/pgm/${m}_${i}_${j}.rle";
      ./a.out "img_result/alcazar/ppm/${m}_${i}_${j}.ppm" "img_result/alcazar/rle/ppm/${m}_${i}_${j}.rle";
    done
  done
done

#JUNGLE LIL
for m in "${n_mode[@]}"
do
  for i in "${n_sp_jungle[@]}"
  do
    for j in "${n_compactness[@]}"
    do
      ./a.out "img_result/jungle/pgm/${m}_${i}_${j}.pgm" "img_result/jungle/rle/pgm/${m}_${i}_${j}.rle";
      ./a.out "img_result/jungle/ppm/${m}_${i}_${j}.ppm" "img_result/jungle/rle/ppm/${m}_${i}_${j}.rle";
    done
  done
done

############################################################################################################
############################################# HUFFMAN ######################################################
############################################################################################################
g++ Huffman.cpp;

#CHAMELEON
for m in "${n_mode[@]}"
do
  for i in "${n_sp_chameleon[@]}"
  do
    for j in "${n_compactness[@]}"
    do
      ./a.out c "img_result/chameleon/rle/pgm/${m}_${i}_${j}.rle" "img_result/chameleon/huffman/pgm/${m}_${i}_${j}.huf";
      ./a.out c "img_result/chameleon/rle/ppm/${m}_${i}_${j}.rle" "img_result/chameleon/huffman/ppm/${m}_${i}_${j}.huf";
    done
  done
done


#ALCAZAR LIL
for m in "${n_mode[@]}"
do
  for i in "${n_sp_alcazar[@]}"
  do
    for j in "${n_compactness[@]}"
    do
      ./a.out "c" "img_result/alcazar/rle/pgm/${m}_${i}_${j}.rle" "img_result/alcazar/huffman/pgm/${m}_${i}_${j}.huf";
      ./a.out "c" "img_result/alcazar/rle/ppm/${m}_${i}_${j}.rle" "img_result/alcazar/huffman/ppm/${m}_${i}_${j}.huf";
    done
  done
done

#JUNGLE LIL
for m in "${n_mode[@]}"
do
  for i in "${n_sp_jungle[@]}"
  do
    for j in "${n_compactness[@]}"
    do
      ./a.out "c" "img_result/jungle/rle/pgm/${m}_${i}_${j}.rle" "img_result/jungle/huffman/pgm/${m}_${i}_${j}.huf";
      ./a.out "c" "img_result/jungle/rle/ppm/${m}_${i}_${j}.rle" "img_result/jungle/huffman/ppm/${m}_${i}_${j}.huf";
    done
  done
done