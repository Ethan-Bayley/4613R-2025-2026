#!/bin/bash

branch=$(git branch --show-current)
last_commit=$(git rev-parse --short HEAD)
date=$(date)

desc="[${branch}] <${last_commit}> ${date}"

echo "UPLOADING: ${desc}"

pros upload --description "${desc}"
