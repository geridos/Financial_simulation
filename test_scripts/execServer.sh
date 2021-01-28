#!/bin/bash

echo going to execute server with example parameters...
export MODE=2;  ../bin/server --input ../market_data.csv --thread 3
