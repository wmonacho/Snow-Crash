# Snow Crash Project

## Description
A brief description of the Snow Crash project.

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Commands](#commands)
- [Contributing](#contributing)
- [License](#license)

## Installation
Instructions on how to install the project.
installer l'iso et creer une VM avec

## Usage
on essaie de trouver le mdp du user flag00
on cherche avec finds ses fichiers
avec la commande : find / -user flag00
ou on peut rajouter 2>/dev/null pour sortir toutes les erreurs des permissions denied dans le void lol
ici on trouve deux dossiers john avec le meme contenue, "cdiiddwpgswtgt"
le mdp ne marche pas donc il faut trouver des astuces. sur internet on peut voir ce que veut dire john pour john the riper et on cherche des chiffrement de mdp et on tombe assez vite sur le chiffrement de cesar avec https://www.dcode.fr/chiffre-cesar pour tous les faire en meme temps et trouver une coherence

token pour le prochain level : x24ti5gi3x0ol2eh4esiuxias

## Commands
A list of useful commands for the project.

To copy files from the VM to the host machine
```bash
scp level00@127.0.0.1:~/sgoinfre/mon_fichier.txt ~/Document/Project_42/In_Progress/Snow-Crash/
```

## Contributing
Guidelines for contributing to the project.

## License
Information about the project's license.