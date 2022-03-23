# ENCE361 Group 1
Matthew Suter, Tim Preston-Marshall, Daniel Rabbidge

Set heap and stack sizes to 512 bytes!

# Sampling Rate Notes/Working
## Pot
Testing in the lab, when using the interrput sampling rate, a buffer <5 values long leads to 'wobble' in the readout
5 is still pretty responsive to change, so no comprimise appears necessacry

## Acc
Maths suggests if walking is <3Hz, the buffer must hold >1/6 of a second of data
Sam seems to be suggesting that we can just pick one and then use an appropriate value for the other
6 Hz is the min, if using Fourier transforms. That is too much work, so we'll make it way bigger. Let's say 60 Hz?
That would suggest a 10-value buffer

# Command line instructions

You can also upload existing files from your computer using the instructions below.
Git global setup

```
git config --global user.name "Matthew Suter"
git config --global user.email "mrs163@uclive.ac.nz"
```

## Create a new repository

```
git clone git@eng-git.canterbury.ac.nz:ence361-2022/group1.git
cd group4
git switch -c main
touch README.md
git add README.md
git commit -m "add README"
git push -u origin main
```

## Push an existing folder

```
cd existing_folder
git init --initial-branch=main
git remote add origin git@eng-git.canterbury.ac.nz:ence361-2022/group1.git
git add .
git commit -m "Initial commit"
git push -u origin main
```

## Push an existing Git repository

```
cd existing_repo
git remote rename origin old-origin
git remote add origin git@eng-git.canterbury.ac.nz:ence361-2022/group1.git
git push -u origin --all
git push -u origin --tags
```

