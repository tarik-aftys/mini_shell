# mini_shell
Mini-shell interactif en C, permettant l'exécution de commandes Unix avec support des redirections, pipes multiples, historique des commandes et commandes internes.
---

##  Prérequis

- **Bibliothèque GNU Readline**  
  Nécessaire pour l’historique et les touches fléchées.  
  Installer sur Ubuntu/Debian avec :

  ```bash
  sudo apt update
  sudo apt install libreadline-dev
  ```

- **Compilateur GCC**  
  Pour compiler les fichiers sources C.

---

##  Fonctionnalités

### 🔹 Prompt
- Affiche le nom de l'utilisateur et le répertoire courant
- Couleurs pour une meilleure lisibilité

### 🔹 Exécution de commandes
- Exécution de commandes externes via `execvp()`
- Lancement de processus en arrière-plan avec `&`

### 🔹 Analyse et opérateurs
- Tubes (pipes) avec `|`
- Redirection de l’entrée : `<`
- Redirection de la sortie : `>`
- Exécution en arrière-plan : `&`

### 🔹 Commandes internes disponibles

| Commande     | Description                                   |
|--------------|-----------------------------------------------|
| `cd`         | Changer de répertoire                         |
| `pwd`        | Afficher le répertoire courant                |
| `clear`      | Effacer le terminal                           |
| `exit`       | Quitter le shell                              |
| `help`       | Afficher les commandes internes disponibles   |
| `history`    | Afficher l’historique (via GNU Readline)      |

---

##  Compilation et exécution

### 🔹 Pour compiler le projet :

```bash
make
```

### 🔹 Pour lancer le shell :

```bash
./mini_shell
```


---

##  Exemples d'utilisation

```bash
# Lister les fichiers
ls -l

# Utiliser un pipe
ls | grep .c

# Rediriger la sortie vers un fichier
echo Bonjour > fichier.txt

# Lire depuis un fichier
cat < fichier.txt

# Lancer un processus en arrière-plan
sleep 5 &
```

✔ Utilisez les flèches ↑ ↓ pour parcourir l’historique  
✔ Combinez les redirections et les pipes librement (`cat fichier | grep txt > résultat.txt`)

---

##  Structure du projet

```
mini_shell/
├── main.c         # Boucle principale du shell et gestion des commandes internes
├── parser.c       # Analyse et découpage des lignes de commande
├── executor.c     # Exécution, redirection, gestion des pipes
├── shell.h        # Fichier d’en-tête commun
├── Makefile       # Script de compilation
├── .gitignore     # Fichiers à ignorer (binaire, temporaires, etc.)
└── README.md      # Documentation du projet
```

---
