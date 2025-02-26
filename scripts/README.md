
Here's a **Bash script** that configures Git submodules to use **Tony's registry** when available, and **GitHub otherwise**. It also includes options to **set, check, or remove** the override.

### 📌 **How to Use the Script**
1. **Save the script** as `git-submodule-switcher.sh` and make it executable:
   ```bash
   chmod +x git-submodule-switcher.sh
   ```
2. **Run the script** with one of the following options:

   - **Set Tony's registry as the preferred source:**
     ```bash
     ./git-submodule-switcher.sh set
     ```
   - **Check current Git override settings:**
     ```bash
     ./git-submodule-switcher.sh check
     ```
   - **Remove the override and restore default GitHub URLs:**
     ```bash
     ./git-submodule-switcher.sh remove
     ```
   - **Sync and update submodules (regardless of the override):**
     ```bash
     ./git-submodule-switcher.sh update
     ```

Now you can easily switch between **Tony’s local registry** and **GitHub** dynamically. 🚀 Let me know if you need modifications!