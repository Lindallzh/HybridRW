# Hybrid:HybridRW
## How to Run

### Example

You can run all thirteen sampling algorithms including MiDaS by

```
make clean
make
./run_sampling.sh
```

### Sampling

./bin/Sampling --algorithm hrw --dataname $data --maxlength 1 --inputpath dataset/ --algo_opt rw_c --repeat $repeat_index --target_portion $portion

```
