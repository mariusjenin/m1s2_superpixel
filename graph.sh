#!/bin/bash

source values_bash.sh

# IN GNUPLOT
#set xlabel "Compactness rate"
#set ylabel "Number of superpixels"
#set zlabel "Compression rate\nor\nPSNR"
#set dgrid3d 50,50
#set ticslevel 0
#set grid x y z vertical lw 1
#sp 'graph/chameleon/pgm_0.dat' u 1:2:4 w l t "Compression rate", 'graph/chameleon/pgm_0.dat' u 1:2:3 w l t "PSNR"

g++ psnr.cpp -o psnr

#GRAPH CHAMELEON
bit_size_base_pgm="$(du -b img/chameleon.pgm | cut -f1)"
bit_size_base_ppm="$(du -b img/chameleon.ppm | cut -f1)"
# shellcheck disable=SC2027
# shellcheck disable=SC2154
# shellcheck disable=SC2129
for m in "${n_mode[@]}"; do
  echo -n "" >"graph/chameleon/pgm_"$m".dat"
  echo -n "" >"graph/chameleon/ppm_"$m".dat"
  for i in "${n_sp_chameleon[@]}"; do
    for j in "${n_compactness[@]}"; do
      bit_size_read="$(du -b img_result/chameleon/huffman/pgm/"$m"_"$i"_"$j".huf | cut -f1)"
      compression="$(bc <<<"scale=5; $bit_size_base_pgm / $bit_size_read")"
      echo -n "$j" >>"graph/chameleon/pgm_"$m".dat"
      echo -n " $i " >>"graph/chameleon/pgm_"$m".dat"
      ./psnr "img/chameleon.pgm" "img_result/chameleon/pgm/""$m""_""$i""_""$j"".pgm" >>"graph/chameleon/pgm_"$m".dat"
      echo " $compression" >>"graph/chameleon/pgm_"$m".dat"


      bit_size_read="$(du -b img_result/chameleon/huffman/ppm/"$m"_"$i"_"$j".huf | cut -f1)"
      compression="$(bc <<<"scale=5; $bit_size_base_ppm / $bit_size_read")"
      echo -n "$j" >>"graph/chameleon/ppm_"$m".dat"
      echo -n " $i " >>"graph/chameleon/ppm_"$m".dat"
      ./psnr "img/chameleon.ppm" "img_result/chameleon/ppm/""$m""_""$i""_""$j"".ppm" >>"graph/chameleon/ppm_"$m".dat"
      echo " $compression" >>"graph/chameleon/ppm_"$m".dat"
    done
  done
done

#GRAPH ALCAZAR
bit_size_base_pgm="$(du -b img/alcazar_lil.pgm | cut -f1)"
bit_size_base_ppm="$(du -b img/alcazar_lil.ppm | cut -f1)"
# shellcheck disable=SC2027
# shellcheck disable=SC2154
# shellcheck disable=SC2129
for m in "${n_mode[@]}"; do
  echo -n "" >"graph/alcazar/pgm_"$m".dat"
  echo -n "" >"graph/alcazar/ppm_"$m".dat"
  for i in "${n_sp_alcazar[@]}"; do
    for j in "${n_compactness[@]}"; do
      bit_size_read="$(du -b img_result/alcazar/huffman/pgm/"$m"_"$i"_"$j".huf | cut -f1)"
      compression="$(bc <<<"scale=5; $bit_size_base_pgm / $bit_size_read")"
      echo -n "$j" >>"graph/alcazar/pgm_"$m".dat"
      echo -n " $i " >>"graph/alcazar/pgm_"$m".dat"
      ./psnr "img/alcazar_lil.pgm" "img_result/alcazar/pgm/""$m""_""$i""_""$j"".pgm" >>"graph/alcazar/pgm_"$m".dat"
      echo " $compression" >>"graph/alcazar/pgm_"$m".dat"


      bit_size_read="$(du -b img_result/alcazar/huffman/ppm/"$m"_"$i"_"$j".huf | cut -f1)"
      compression="$(bc <<<"scale=5; $bit_size_base_ppm / $bit_size_read")"
      echo -n "$j" >>"graph/alcazar/ppm_"$m".dat"
      echo -n " $i " >>"graph/alcazar/ppm_"$m".dat"
      ./psnr "img/alcazar_lil.ppm" "img_result/alcazar/ppm/""$m""_""$i""_""$j"".ppm" >>"graph/alcazar/ppm_"$m".dat"
      echo " $compression" >>"graph/alcazar/ppm_"$m".dat"
    done
  done
done

#GRAPH JUNGLE
bit_size_base_pgm="$(du -b img/jungle_lil.pgm | cut -f1)"
bit_size_base_ppm="$(du -b img/jungle_lil.ppm | cut -f1)"
# shellcheck disable=SC2027
# shellcheck disable=SC2154
# shellcheck disable=SC2129
for m in "${n_mode[@]}"; do
  echo -n "" >"graph/jungle/pgm_"$m".dat"
  echo -n "" >"graph/jungle/ppm_"$m".dat"
  for i in "${n_sp_jungle[@]}"; do
    for j in "${n_compactness[@]}"; do
      bit_size_read="$(du -b img_result/jungle/huffman/pgm/"$m"_"$i"_"$j".huf | cut -f1)"
      compression="$(bc <<<"scale=5; $bit_size_base_pgm / $bit_size_read")"
      echo -n "$j" >>"graph/jungle/pgm_"$m".dat"
      echo -n " $i " >>"graph/jungle/pgm_"$m".dat"
      ./psnr "img/jungle_lil.pgm" "img_result/jungle/pgm/""$m""_""$i""_""$j"".pgm" >>"graph/jungle/pgm_"$m".dat"
      echo " $compression" >>"graph/jungle/pgm_"$m".dat"


      bit_size_read="$(du -b img_result/jungle/huffman/ppm/"$m"_"$i"_"$j".huf | cut -f1)"
      compression="$(bc <<<"scale=5; $bit_size_base_ppm / $bit_size_read")"
      echo -n "$j" >>"graph/jungle/ppm_"$m".dat"
      echo -n " $i " >>"graph/jungle/ppm_"$m".dat"
      ./psnr "img/jungle_lil.ppm" "img_result/jungle/ppm/""$m""_""$i""_""$j"".ppm" >>"graph/jungle/ppm_"$m".dat"
      echo " $compression" >>"graph/jungle/ppm_"$m".dat"
    done
  done
done
