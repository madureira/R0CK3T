@echo off

call git submodule update --init
cd Libraries\boost
call git submodule update --init --recursive
