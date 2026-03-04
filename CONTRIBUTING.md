# Contributing Guide

Welcome, and thank you for your interest in contributing! Please read this guide carefully before making any changes. Following these steps will help you avoid the most common mistakes that new contributors make, particularly **merge conflicts**.

---

## What is a Merge Conflict and Why Should You Care?

A merge conflict happens when two people edit the same part of the same file at the same time, and Git doesn't know which version to keep. They are painful to fix and slow everyone down. The steps below are designed specifically to help you avoid them.

---

## Before You Start — The Golden Rule

> **Never work directly on the `main` branch. Ever.**

The `main` branch is the stable version of the project. All changes must go through a **Pull Request (PR)** from your own branch.

---

## Step-by-Step: How to Make a Pull Request

### 1. Fork the Repository
Click the **Fork** button at the top right of this GitHub page. This creates your own personal copy of the project under your GitHub account.

### 2. Clone Your Fork Locally
```bash
git clone https://github.com/YOUR_USERNAME/REPO_NAME.git
cd REPO_NAME
```

### 3. Add the Original Repo as "upstream"
This lets you pull in new changes from the main project later.
```bash
git remote add upstream https://github.com/ORIGINAL_OWNER/REPO_NAME.git
```

### 4. Always Sync Before Starting Work
Before touching any code, make sure your local copy is up to date:
```bash
git checkout main
git pull upstream main
```
Skipping this step is the #1 cause of merge conflicts. Do not skip it.

### 5. Create a New Branch for Your Work
Never work on `main`. Create a new branch with a descriptive name:
```bash
git checkout -b feature/your-feature-name
```

### 6. Make Your Changes
Edit files, write code, do your work. Keep your changes focused — one branch per feature or fix.

### 7. Commit Your Changes
```bash
git add .
git commit -m "Short description of what you changed"
```
Write commit messages that explain **what** and **why**, not just "fixed stuff".

### 8. Push Your Branch to GitHub
```bash
git push origin feature/your-feature-name
```

### 9. Open a Pull Request
- Go to your fork on GitHub
- Click **"Compare & pull request"**
- Write a clear description of what your PR does and why
- Submit it and wait for review

---

## If Your PR Has Conflicts

If GitHub tells you there are merge conflicts, it means the `main` branch changed while you were working. Fix it like this:

```bash
git checkout main
git pull upstream main
git checkout feature/your-feature-name
git merge main
```

Git will show you exactly which lines conflict. Resolve them, then:
```bash
git add .
git commit -m "Resolved merge conflicts"
git push origin feature/your-feature-name
```

---

## General Rules

- One pull request per feature or bug fix — do not bundle unrelated changes
- Do not commit directly to `main`
- Do not modify files unrelated to your change
- Always sync with `upstream main` before starting new work
- If you are unsure about something, open an Issue and ask before writing code

---

## Quick Reference

```
fork → clone → add upstream → sync → new branch → work → commit → push → pull request
```

This order matters. Following it consistently will save you and the maintainers a significant amount of time.

---

*This guide was written by [Claude](https://claude.ai), Anthropic's AI assistant — on behalf of the project maintainers.*
