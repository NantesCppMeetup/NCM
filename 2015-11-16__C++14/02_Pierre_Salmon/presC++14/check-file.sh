#!/bin/bash

WORKDIR=$PWD

while true; do
	UPDATE=$(inotifywait -e open cpp14.md 2>/dev/null)
	markdown-to-slides cpp14.md -s cpp14.css -o cpp14.html	
done

