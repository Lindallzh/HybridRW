dataset=("email-Eu-full" "coauth-MAG-Geology-full" "contact-primary-school" "threads-ask-ubuntu" "tags-ask-ubuntu")
searchspace=("0.0" "0.125" "0.1768" "0.25" "0.3536" "0.5" "0.7071" "1.0" "1.4142" "2.0" "2.8284" "4.0" "5.6569" "8.0" "11.3137" "16.0" "22.6274" "32.0" "45.2548" "64.0")

for repeat_index in 1 2
do
    for portion in 0.1 0.2 0.3 0.4 0.5
    do
        for data in ${dataset[@]}
        do
            # MiDaS & MiDaS_Basic
          
            # Simple and Intuitive Approaches 
            ./bin/Sampling --algorithm rw --dataname $data --maxlength 1 --inputpath dataset/ --algo_opt rw_c --repeat $repeat_index --target_portion $portion
            ./bin/Sampling --algorithm hrw --dataname $data --maxlength 1 --inputpath dataset/ --algo_opt rw_c --repeat $repeat_index --target_portion $portion
            ./bin/Sampling --algorithm nobackhrw --dataname $data --maxlength 1 --inputpath dataset/ --algo_opt rw_c --repeat $repeat_index --target_portion $portion  
            ./bin/Sampling --algorithm skiphrw --dataname $data --maxlength 1 --inputpath dataset/ --algo_opt rw_c --repeat $repeat_index --target_portion $portion
        done
    done
done

#./bin/Sampling --algorithm rw --dataname coauth-MAG-Geology-full --maxlength 1 --inputpath dataset/ --algo_opt rw_c --repeat 1 --target_portion 0.1
