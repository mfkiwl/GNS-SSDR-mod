# GNSS receiver with spoofing detection

The goal of this project is to include a spoofing detection system into
the GNSS-SDR receiver

## Branches
* **master**:
current stable version

* **dev:**
most stable development version

* **experimental:**
works in progress and testing

* **gnss-sdr_original:**
original source of GNSS-SDR

## Directories

* **GNSS_SDR_CORE:**
the modified GNSS-SDR code

* **C45x:**:
A decision tree construction program based on the c4.5 algorithm

* **report:**
A LATEX document desccribing the project

* **test:**
directory where to run the modified GNSS-SDR (assuming it is installed)

## Istallation

from /GNSS-SDR_CORE/build run
* cmake ..
* make
* sudo make install

## Running
gnss-sdr --config_file=\<fileName\>