# ListPart

ListPart es una herramienta para listar particiones de discos duros con esquemas de particionado MBR/GPT.

## Características

- Listar particiones de discos con esquema MBR.
- Listar particiones de discos con esquema GPT.
- Mostrar información detallada de cada partición, incluyendo el tipo y el nombre de la partición.

## Requisitos

- GCC (GNU Compiler Collection)
- Make

## Instalación

Para compilar e instalar ListPart, sigue estos pasos:

1. Clona el repositorio:
    ```sh
    git clone <URL_DEL_REPOSITORIO>
    cd <NOMBRE_DEL_REPOSITORIO>
    ```

2. Compila el proyecto:
    ```sh
    make
    ```

3. Instala la herramienta:
    ```sh
    sudo make install
    ```

## Uso

Para usar ListPart, ejecuta el siguiente comando:

```sh
listpart <ruta_al_disco>
```

Por ejemplo:

```sh
listpart /dev/sda
```

## Estructura del Proyecto

```
.gitignore
gpt.c
gpt.h
LICENSE
main.c
Makefile
mbr.c
mbr.h
README.md
```

## Descripción de Archivos

- `gpt.c` y `gpt.h`: Implementación y definiciones para discos inicializados con esquema GPT.
- `mbr.c` y `mbr.h`: Implementación y definiciones para discos inicializados con esquema MBR.
- `main.c`: Archivo principal que contiene la lógica para listar las particiones.
- `Makefile`: Archivo para compilar y construir el proyecto.
- `.vscode/`: Configuraciones específicas para Visual Studio Code.
- `LICENSE`: Licencia MIT para el proyecto.
- `README.md`: Este archivo de documentación.

## Contribuciones

Las contribuciones son bienvenidas. Por favor, sigue estos pasos para contribuir:

1. Haz un fork del repositorio.
2. Crea una nueva rama (`git checkout -b feature/nueva-funcionalidad`).
3. Realiza tus cambios y haz commit (`git commit -am 'Añadir nueva funcionalidad'`).
4. Sube tus cambios a tu fork (`git push origin feature/nueva-funcionalidad`).
5. Abre un Pull Request.

## Licencia

Este proyecto está licenciado bajo la Licencia MIT. Consulta el archivo LICENSE para más detalles.

## Autores

- Erwin Meza Vega: emezav@unicauca.edu.co
- Miguel Angel Calambas: mangelcvivas@unicauca.edu.co
- Esteban Santiago Escadon Causaya: estebanescandon@unicauca.edu.co