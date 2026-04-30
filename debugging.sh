#!/usr/bin/env bash

fd . src/ | entr -c bash -c "make debug && make run"
