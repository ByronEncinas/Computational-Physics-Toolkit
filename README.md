# This is the 'dev' branch

Here all changes must tested and be well-behaved before any merge with main.

- If any pull request is submitted on main branch, this will be ignored.
- Please comment your changes on your commits. Advice, use:

	git commit -m "Bug Fix|Feature|Format|etc: Detailed Explanation" -m "Even more detail"

Communicate with the official maintainer if you have any questions. 


<<<<<<< HEAD

# Maintainer Guide

*Written by [Claude](https://claude.ai), Anthropic's AI assistant.*

---

## How to Compile
=======
# This is the 'dev' branch

Here all changes must tested and be well-behaved before any merge with main.

- If any pull request is submitted on main branch, this will be ignored.
- Please comment your changes on your commits. Advice, use:

	git commit -m "Bug Fix|Feature|Format|etc: Detailed Explanation" -m "Even more detail"

Communicate with the official maintainer if you have any questions. 
>>>>>>> 2a2e0688b78e117d6a3d6d6a618928950235f231

To create the executable `./main`:

```bash
cd build
cmake ..
make
./main
```

---

## Contribution Workflow

### 1. Clone the repository
```bash
git clone https://github.com/OWNER/REPO_NAME.git
cd REPO_NAME
```

### 2. Create your personal development branch
The owner's branch is called `dev` — choose a different name for yours:
```bash
git checkout -b feature/your-name
```

### 3. Make your changes
- Keep changes focused — **one feature or fix per branch**
- Try to limit edits to a single file when possible
- Excessive changes across the whole repository will likely cause merge conflicts and you will be asked to rewrite your contribution

### 4. Commit and push your branch
```bash
git add .
git commit -m "Short description of what you changed"
git push origin feature/your-name
```

### 5. Open a Pull Request
Push your branch to GitHub and open a Pull Request targeting the `dev` branch. The repository owner will review it and decide whether to merge.

---

## Daily Workflow (after first setup)

```bash
# Always sync before starting work
git checkout feature/your-name
git pull origin dev

# Do your work, then commit and push
git add .
git commit -m "your message" -m "if needed give more detailed explanation here"
git push origin feature/your-name
```

---

## Branch Structure

```
main                  ← stable release, do not touch
dev                   ← owner's integration branch
feature/your-name     ← your personal working branch
```

---

## Rules

- Never commit directly to `main` or `dev`
- Always pull before starting new work (`git pull`)
- One pull request per feature or fix — do not bundle unrelated changes
- If your PR has conflicts, resolve them locally before requesting a review

