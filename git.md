# Git and GitHub Guide

A practical guide to get you started with Git and GitHub from scratch.

## Table of Contents

1. [What is Git?](#1-what-is-git)
2. [What is GitHub?](#2-what-is-github)
3. [Why Should You Learn Git?](#3-why-should-you-learn-git)
4. [Installing Git](#4-installing-git)
5. [First Time Setup](#5-first-time-setup)
6. [Basic Concepts](#6-basic-concepts)
7. [Your First Repository](#7-your-first-repository)
8. [Daily Git Commands](#8-daily-git-commands)
9. [Working with Branches](#9-working-with-branches)
10. [Using GitHub](#10-using-github)
11. [Common Problems and Solutions](#11-common-problems-and-solutions)
12. [Practice Exercises](#12-practice-exercises)
13. [Quick Command Reference](#13-quick-command-reference)

---

## 1) What is Git?

**Git** is a tool that helps you track changes in your files. Think of it like a "save game" feature in video games - you can always go back to a previous version if something goes wrong.

**Real-world example**: Imagine writing an essay. Instead of saving multiple files like:
- essay_v1.docx
- essay_v2_final.docx
- essay_v2_final_REALLY_FINAL.docx

Git lets you keep one file and tracks all versions automatically!

---

## 2) What is GitHub?

**GitHub** is a website where you can store your Git projects online. It's like Google Drive, but specifically designed for code and Git projects.

**Key difference**:
- **Git** = The tool on your computer
- **GitHub** = The website for sharing and backup

```
┌─────────────────────────────────────────────────┐
│                                                 │
│  Your Computer          →          GitHub       │
│  (Git installed)                   (Website)    │
│                                                 │
│  Work on files         →          Share work    │
│  Save versions         →          Collaborate   │
│  Work offline          →          Backup        │
│                                                 │
└─────────────────────────────────────────────────┘
```

---

## 3) Why Should You Learn Git?

**Save your work history** - Never lose your previous work  
**Work with others** - Multiple people can work on the same project  
**Try new ideas safely** - Experiment without breaking your code  
**Required for most jobs** - Nearly all companies use Git  
**Share your projects** - Show your work to the world  

---

## 4) Installing Git

### Step 1: Download Git

Visit the official Git website:  
**https://git-scm.com/downloads**

### Step 2: Install

**Windows**:
1. Download the installer from https://git-scm.com/download/win
2. Run the installer
3. Click "Next" through all options (defaults are fine)

**Mac**:
1. Open Terminal
2. Type: `git --version`
3. If not installed, it will prompt you to install (or use `brew install git`)

**Linux (Ubuntu/Debian)**:
```bash
sudo apt update
sudo apt install git
```

### Step 3: Verify Installation

Open your terminal (Command Prompt on Windows, Terminal on Mac/Linux) and type:

```bash
git --version
```

You should see something like: `git version 2.40.0`

---

## 5) First Time Setup

Before using Git, tell it who you are:

```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

**Example**:
```bash
git config --global user.name "John Doe"
git config --global user.email "john@example.com"
```

**Check your settings**:
```bash
git config --list
```

---

## 6) Basic Concepts

### The Three Areas in Git

```
┌─────────────────────────────────────────────────────────┐
│                                                         │
│  1. WORKING DIRECTORY      (Your project folder)        │
│     You edit files here                                 │
│                    ↓                                    │
│              git add <file>                             │
│                    ↓                                    │
│  2. STAGING AREA           (Files ready to save)        │
│     Files waiting to be saved                           │
│                    ↓                                    │
│              git commit                                 │
│                    ↓                                    │
│  3. REPOSITORY             (Saved snapshots)            │
│     Your project history                                │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### Important Terms

| Term | Simple Explanation |
|------|-------------------|
| **Repository (repo)** | Your project folder tracked by Git |
| **Commit** | A saved snapshot of your project |
| **Branch** | A separate version of your project to work on |
| **Remote** | Your project stored online (like on GitHub) |
| **Clone** | Download a project from GitHub |
| **Push** | Upload your changes to GitHub |
| **Pull** | Download changes from GitHub |

### How Git Tracks Changes

```
Time flows from left to right →

○───○───○───○───○
v1  v2  v3  v4  v5

Each ○ is a commit (saved version)
You can go back to any version anytime!
```

---

## 7) Your First Repository

Let's create your first Git project step by step!

### Method 1: Start a New Project

```bash
# Step 1: Create a folder
mkdir my-first-project
cd my-first-project

# Step 2: Initialize Git
git init

# Step 3: Create a file
echo "Hello Git!" > hello.txt

# Step 4: Check status
git status

# Step 5: Add file to staging
git add hello.txt

# Step 6: Save (commit)
git commit -m "Add hello file"
```

**What you'll see**:
```
Initialized empty Git repository in /path/to/my-first-project/.git/
```

Congratulations! You just created your first Git repository! 🎉

---

## 8) Daily Git Commands

### Check What Changed

```bash
git status
```

**This shows**:
- Modified files (red)
- Files ready to commit (green)
- New files not tracked yet

### Add Files to Staging

```bash
# Add one file
git add filename.txt

# Add all files
git add .

# Add multiple specific files
git add file1.txt file2.txt
```

### Save Your Changes (Commit)

```bash
git commit -m "Describe what you changed"
```

**Examples of good commit messages**:
```bash
git commit -m "Add login page"
git commit -m "Fix typo in README"
git commit -m "Update contact information"
```

### View Your History

```bash
# Simple view
git log --oneline

# Detailed view
git log
```

### See What You Changed

```bash
# See unstaged changes
git diff

# See staged changes
git diff --staged
```

### Undo Changes

```bash
# Undo changes in a file (before adding)
git restore filename.txt

# Unstage a file
git restore --staged filename.txt

# Undo last commit (keep changes)
git reset --soft HEAD~1
```

---

## 9) Working with Branches

### What are Branches?

Branches let you work on new features without affecting your main code.

```
main:      ○───○───○───────────○
                \             /
feature:         ○───○───○───○

You can work on 'feature' safely,
and merge it into 'main' when ready!
```

### Branch Commands

```bash
# See all branches
git branch

# Create a new branch
git branch feature-name

# Switch to a branch
git switch feature-name

# Create and switch in one step
git switch -c feature-name

# Go back to main branch
git switch main

# Merge a branch into current branch
git merge feature-name

# Delete a branch
git branch -d feature-name
```

### Example Workflow

```bash
# 1. Start from main
git switch main

# 2. Create feature branch
git switch -c add-contact-page

# 3. Make changes and commit
echo "Contact Page" > contact.html
git add contact.html
git commit -m "Add contact page"

# 4. Go back to main
git switch main

# 5. Merge your feature
git merge add-contact-page

# 6. Delete feature branch
git branch -d add-contact-page
```

---

## 10) Using GitHub

### Step 1: Create a GitHub Account

Go to **https://github.com** and sign up for free!

### Step 2: Create a New Repository on GitHub

1. Click the **+** button (top right)
2. Click **"New repository"**
3. Give it a name (example: `my-project`)
4. Click **"Create repository"**

### Step 3: Connect Your Local Project to GitHub

GitHub will show you commands. Use these:

```bash
# Add GitHub as remote
git remote add origin https://github.com/YOUR-USERNAME/my-project.git

# Push your code
git push -u origin main
```

### Common GitHub Commands

```bash
# Download a project from GitHub
git clone https://github.com/username/project.git

# Upload your changes
git push

# Download latest changes
git pull

# See your remotes
git remote -v
```

### Collaboration Workflow

```
┌────────────────────────────────────────────────────┐
│                                                    │
│  1. Clone       →  Download project                │
│  2. Branch      →  Create feature branch           │
│  3. Work        →  Make changes                    │
│  4. Commit      →  Save changes locally            │
│  5. Push        →  Upload to GitHub                │
│  6. Pull Request→  Ask to merge your work          │
│  7. Review      →  Others review your code         │
│  8. Merge       →  Add your work to main project   │
│                                                    │
└────────────────────────────────────────────────────┘
```

### Creating a Pull Request

1. Push your branch to GitHub:
   ```bash
   git push origin feature-branch
   ```

2. Go to GitHub website
3. Click **"Compare & pull request"**
4. Write description of changes
5. Click **"Create pull request"**

---

## 11) Common Problems and Solutions

### Problem 1: "I made a mistake in my last commit"

**Solution**:
```bash
# Fix and add to last commit
git add fixed-file.txt
git commit --amend --no-edit
```

### Problem 2: "I want to undo my changes"

**Solution**:
```bash
# Undo changes in one file
git restore filename.txt

# Undo all changes
git restore .
```

### Problem 3: "I'm on the wrong branch!"

**Solution**:
```bash
# Save your work temporarily
git stash

# Switch to correct branch
git switch correct-branch

# Get your work back
git stash pop
```

### Problem 4: "Git says I have conflicts"

**What happened**: You and someone else changed the same line.

**Solution**:
1. Open the file
2. Look for these markers:
   ```
   <<<<<<< HEAD
   Your changes
   =======
   Their changes
   >>>>>>> branch-name
   ```
3. Edit to keep what you want
4. Remove the markers (`<<<<<<<`, `=======`, `>>>>>>>`)
5. Save and commit:
   ```bash
   git add filename.txt
   git commit -m "Resolve conflict"
   ```

### Problem 5: "I can't push to GitHub"

**Solution**:
```bash
# First pull the latest changes
git pull

# Then push
git push
```

### Problem 6: "I accidentally deleted something"

**Solution**:
```bash
# Find what you lost
git reflog

# Recover it
git checkout <commit-hash>
```

---

## 12) Practice Exercises

### Exercise 1: Basic Git Workflow

**Goal**: Learn the basic save cycle

```bash
# 1. Create a project
mkdir git-practice
cd git-practice
git init

# 2. Create a file
echo "My first project" > README.md

# 3. Check status
git status

# 4. Add and commit
git add README.md
git commit -m "Initial commit"

# 5. Make a change
echo "This is awesome!" >> README.md

# 6. See what changed
git diff

# 7. Commit the change
git add README.md
git commit -m "Update README"

# 8. View history
git log --oneline
```

### Exercise 2: Working with Branches

**Goal**: Create a feature on a separate branch

```bash
# 1. Create and switch to new branch
git switch -c add-features

# 2. Create new files
echo "Feature 1" > feature1.txt
echo "Feature 2" > feature2.txt

# 3. Commit them
git add .
git commit -m "Add new features"

# 4. Switch back to main
git switch main

# 5. Notice features don't exist here
ls

# 6. Merge the feature
git merge add-features

# 7. Now they exist
ls
```

### Exercise 3: Using GitHub

**Goal**: Upload your project to GitHub

```bash
# 1. Create empty repo on GitHub (via website)

# 2. Connect your local project
git remote add origin https://github.com/YOUR-USERNAME/YOUR-REPO.git

# 3. Push your code
git push -u origin main

# 4. Check GitHub website - your code is there!
```

### Exercise 4: Collaboration Simulation

**Goal**: Practice the full workflow

```bash
# 1. Clone a repository
git clone https://github.com/YOUR-USERNAME/practice-repo.git
cd practice-repo

# 2. Create feature branch
git switch -c my-feature

# 3. Make changes
echo "New feature" > feature.txt
git add feature.txt
git commit -m "Add feature"

# 4. Push branch
git push origin my-feature

# 5. Create Pull Request on GitHub

# 6. After merge, update local main
git switch main
git pull
```

### Exercise 5: Fixing Mistakes

**Goal**: Practice recovery commands

```bash
# 1. Make a file
echo "Important data" > important.txt
git add important.txt
git commit -m "Add important file"

# 2. "Accidentally" delete it
rm important.txt

# 3. Oh no! Recover it
git restore important.txt

# 4. File is back!
cat important.txt
```

---

## 13) Quick Command Reference

### Setup
```bash
git config --global user.name "Your Name"
git config --global user.email "your@email.com"
```

### Start a Project
```bash
git init                                    # Start new repo
git clone <url>                            # Download repo
```

### Basic Workflow
```bash
git status                                 # Check status
git add <file>                            # Stage file
git add .                                 # Stage all files
git commit -m "message"                   # Save snapshot
git log --oneline                         # View history
```

### Branching
```bash
git branch                                # List branches
git switch <branch>                       # Switch branch
git switch -c <branch>                    # Create & switch
git merge <branch>                        # Merge branch
git branch -d <branch>                    # Delete branch
```

### GitHub
```bash
git remote add origin <url>               # Connect to GitHub
git push origin main                      # Upload
git pull origin main                      # Download
```

### Undo
```bash
git restore <file>                        # Discard changes
git restore --staged <file>               # Unstage file
git commit --amend                        # Fix last commit
git reset --soft HEAD~1                   # Undo last commit
```

---

## Tips for Success

### DO:
- Commit often (small changes are better)
- Write clear commit messages
- Always check `git status` before committing
- Pull before you push
- Create branches for new features

### DON'T:
- Commit passwords or secrets
- Work directly on main branch
- Make huge commits with 100 files
- Force push (`git push --force`) unless you know what you're doing
- Panic! Git can recover almost anything

---

## Need Help?

**Official Documentation**: https://git-scm.com/doc

**GitHub Help**: https://docs.github.com

**Interactive Tutorial**: https://learngitbranching.js.org

**Git Cheat Sheet**: https://education.github.com/git-cheat-sheet-education.pdf

---

## Next Steps

Once you're comfortable with the basics:

1. Practice daily with your own projects
2. Contribute to open-source projects on GitHub
3. Learn about `.gitignore` files
4. Explore GitHub Actions (automation)
5. Learn Git rebase and cherry-pick (advanced)

**Remember**: Everyone was a beginner once. The more you practice, the more natural Git becomes!

---

## Essential Git Commands Explained

This section explains the most important Git commands you'll use every day. Keep this as a quick reference!

### Repository Setup

**`git init`**
- **What it does**: Creates a new Git repository in your current folder
- **When to use**: Starting a brand new project
- **Example**: `git init`
- **Result**: Creates hidden `.git` folder to track your project

**`git clone <URL>`**
- **What it does**: Downloads a complete copy of a repository from GitHub (or other remote)
- **When to use**: Starting to work on an existing project
- **Example**: `git clone https://github.com/user/project.git`
- **Result**: Creates a folder with all project files and history

### Checking Status

**`git status`**
- **What it does**: Shows which files are modified, staged, or untracked
- **When to use**: Before every commit to see what will be saved
- **Example**: `git status`
- **Result**: Lists all changes in your working directory

**`git diff`**
- **What it does**: Shows exactly what changed in your files (line by line)
- **When to use**: To review changes before committing
- **Example**: `git diff` (unstaged) or `git diff --staged` (staged changes)
- **Result**: Displays additions (+) and deletions (-)

**`git log`**
- **What it does**: Shows the history of commits
- **When to use**: To see what was done previously
- **Example**: `git log --oneline` (compact view)
- **Result**: Lists all commits with messages and IDs

### Saving Work

**`git add <file>`**
- **What it does**: Stages files (prepares them to be saved in next commit)
- **When to use**: After making changes you want to save
- **Example**: `git add file.txt` or `git add .` (all files)
- **Result**: Moves files to staging area (shows green in git status)

**`git commit -m "message"`**
- **What it does**: Saves a snapshot of all staged changes with a description
- **When to use**: After staging files you want to permanently save
- **Example**: `git commit -m "Fix login bug"`
- **Result**: Creates a new commit in your local repository

**`git commit -am "message"`**
- **What it does**: Adds all modified tracked files AND commits in one step
- **When to use**: Quick commits when you changed multiple files
- **Example**: `git commit -am "Update documentation"`
- **Note**: Only works for files already tracked by Git (not new files)

### Working with Branches

**`git branch`**
- **What it does**: Lists all branches or creates a new branch
- **When to use**: To see available branches or create new ones
- **Example**: `git branch` (list) or `git branch feature-x` (create)
- **Result**: Shows all branches, with `*` marking current branch

**`git switch <branch>`**
- **What it does**: Switches to a different branch
- **When to use**: Moving between different versions of your project
- **Example**: `git switch main` or `git switch feature-x`
- **Result**: Changes your files to match that branch

**`git switch -c <branch>`** or **`git checkout -b <branch>`**
- **What it does**: Creates a new branch AND switches to it immediately
- **When to use**: Starting work on a new feature
- **Example**: `git switch -c add-login` or `git checkout -b add-login`
- **Result**: New branch is created and you're now working on it

**`git merge <branch>`**
- **What it does**: Combines changes from another branch into your current branch
- **When to use**: Adding finished features back to main branch
- **Example**: `git switch main` then `git merge feature-x`
- **Result**: Changes from feature-x are now in main

**`git branch -d <branch>`**
- **What it does**: Deletes a branch (after it's been merged)
- **When to use**: Cleaning up old feature branches
- **Example**: `git branch -d feature-x`
- **Result**: Branch is removed (only if already merged)

### Working with GitHub (Remote)

**`git remote add origin <URL>`**
- **What it does**: Connects your local repository to GitHub
- **When to use**: First time linking local project to GitHub
- **Example**: `git remote add origin https://github.com/user/repo.git`
- **Result**: Sets up connection to remote repository

**`git push origin <branch>`**
- **What it does**: Uploads your local commits to GitHub
- **When to use**: Sharing your work or backing up
- **Example**: `git push origin main` or `git push origin feature-x`
- **Result**: Your commits appear on GitHub

**`git push -u origin <branch>`**
- **What it does**: Pushes AND sets up tracking (first push only)
- **When to use**: First time pushing a new branch
- **Example**: `git push -u origin main`
- **Result**: After this, you can just use `git push` without branch name

**`git pull origin <branch>`**
- **What it does**: Downloads changes from GitHub and merges them
- **When to use**: Getting latest updates from team members
- **Example**: `git pull origin main`
- **Result**: Your local branch is updated with remote changes

**`git fetch`**
- **What it does**: Downloads changes from GitHub but doesn't merge yet
- **When to use**: Checking what changed before merging
- **Example**: `git fetch origin`
- **Result**: Remote changes downloaded, but your files unchanged

### Undoing Changes

**`git restore <file>`**
- **What it does**: Discards changes in working directory (brings back last committed version)
- **When to use**: Made a mistake and want to undo it
- **Example**: `git restore file.txt` or `git restore .` (all files)
- **Result**: File returns to last committed state (LOSES your changes!)

**`git restore --staged <file>`**
- **What it does**: Unstages a file (removes from staging area, keeps changes)
- **When to use**: Accidentally added wrong file
- **Example**: `git restore --staged file.txt`
- **Result**: File moves from green (staged) to red (unstaged)

**`git reset --soft HEAD~1`**
- **What it does**: Undoes last commit but keeps changes staged
- **When to use**: Want to modify the last commit
- **Example**: `git reset --soft HEAD~1`
- **Result**: Commit is gone, files still staged

**`git reset --mixed HEAD~1`** (default)
- **What it does**: Undoes last commit and unstages files (keeps changes in working directory)
- **When to use**: Want to re-do last commit differently
- **Example**: `git reset HEAD~1`
- **Result**: Commit gone, files unstaged but modified

**`git reset --hard HEAD~1`**
- **What it does**: Completely deletes last commit and all changes
- **When to use**: Last commit was completely wrong (DANGEROUS!)
- **Example**: `git reset --hard HEAD~1`
- **Result**: Everything from last commit is GONE

**`git revert <commit>`**
- **What it does**: Creates a new commit that undoes a previous commit
- **When to use**: Undoing a commit that's already pushed (safe for shared repos)
- **Example**: `git revert abc123`
- **Result**: New commit that reverses the specified commit

### Temporary Storage

**`git stash`**
- **What it does**: Temporarily saves your uncommitted changes and cleans working directory
- **When to use**: Need to switch branches but not ready to commit
- **Example**: `git stash`
- **Result**: Changes saved away, working directory clean

**`git stash pop`**
- **What it does**: Brings back your stashed changes
- **When to use**: Ready to continue working on stashed changes
- **Example**: `git stash pop`
- **Result**: Your changes are restored

**`git stash list`**
- **What it does**: Shows all saved stashes
- **When to use**: You have multiple stashes and need to see them
- **Example**: `git stash list`
- **Result**: Lists all stashes with numbers

**`git stash apply`**
- **What it does**: Applies stashed changes but keeps them in stash list
- **When to use**: Want to apply same changes to multiple branches
- **Example**: `git stash apply`
- **Result**: Changes restored, stash still saved

### File Operations

**`git rm <file>`**
- **What it does**: Deletes file and stages the deletion
- **When to use**: Removing files from project
- **Example**: `git rm old-file.txt`
- **Result**: File deleted and deletion staged for commit

**`git mv <old> <new>`**
- **What it does**: Renames or moves a file and stages the change
- **When to use**: Renaming or moving files
- **Example**: `git mv old.txt new.txt`
- **Result**: File renamed and change staged

### Viewing Information

**`git show <commit>`**
- **What it does**: Shows details of a specific commit
- **When to use**: Want to see exactly what changed in a commit
- **Example**: `git show abc123`
- **Result**: Displays commit message and all changes

**`git blame <file>`**
- **What it does**: Shows who last modified each line of a file
- **When to use**: Finding out who wrote specific code
- **Example**: `git blame file.txt`
- **Result**: Line-by-line history with authors

**`git remote -v`**
- **What it does**: Shows all connected remote repositories
- **When to use**: Checking where your code pushes/pulls from
- **Example**: `git remote -v`
- **Result**: Lists URLs for fetch and push

### Configuration

**`git config --global user.name "Name"`**
- **What it does**: Sets your name for all commits
- **When to use**: First time setup or changing your name
- **Example**: `git config --global user.name "John Doe"`
- **Result**: All future commits show this name

**`git config --global user.email "email"`**
- **What it does**: Sets your email for all commits
- **When to use**: First time setup or changing your email
- **Example**: `git config --global user.email "john@example.com"`
- **Result**: All future commits use this email

**`git config --list`**
- **What it does**: Shows all your Git settings
- **When to use**: Checking your configuration
- **Example**: `git config --list`
- **Result**: Displays all config values

---

### Quick Command Combinations (Common Workflows)

**Save and upload in one go**:
```bash
git add .
git commit -m "Your message"
git push
```

**Start a new feature**:
```bash
git switch -c new-feature
# ... make changes ...
git add .
git commit -m "Add feature"
git push origin new-feature
```

**Update your branch with main**:
```bash
git switch main
git pull
git switch your-branch
git merge main
```

**Quick save work to switch branches**:
```bash
git stash
git switch other-branch
# ... do work ...
git switch original-branch
git stash pop
```

**Undo everything (nuclear option)**:
```bash
git reset --hard HEAD
git clean -fd  # removes untracked files too
```
