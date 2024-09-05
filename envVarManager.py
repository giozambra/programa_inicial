class EnvVarManager:
    '''
    La clase EnvVarManager maneja la carga de variables de entorno desde un archivo .env
    y genera un archivo de encabezado C++ (.hpp) con estas variables definidas como macros.

    Esta clase está diseñada para usarse de manera estática, sin necesidad de crear instancias.
    '''

    @staticmethod
    def load_dotenv(env_file):
        '''
        Carga las variables de entorno desde un archivo .env y las almacena en un diccionario.

        :param env_file: Ruta del archivo .env que contiene las variables de entorno.
        :return: Diccionario con las variables de entorno como pares clave-valor.
        '''

        # Diccionario vacío para almacenar las variables de entorno.
        env_vars = dict()

        # Abre el archivo .env en modo lectura.
        with open(env_file, 'r') as le_file: 
            # Lee todas las líneas del archivo.
            lines = le_file.readlines()
        
        for line in lines:
            if '=' in line:
                # Divide cada línea en clave y valor usando el símbolo '=' como delimitador.
                key, value = line.strip().split('=', 1)
                # Elimina las comillas alrededor del valor, si existen.
                value = value.strip()
                env_vars[key.strip()] = value.strip('"')
        
        # Diccionario con las variables de entorno.
        return env_vars

    @staticmethod
    def generate_header_file(env_vars, output_file):
        '''
        Genera un archivo de encabezado C++ (.hpp)
        con las definiciones de las variables de entorno.

        :param env_vars: Diccionario con las variables de entorno como pares clave-valor.
        :param output_file: Ruta del archivo de salida que contendrá las definiciones.
        '''

        # Abre el archivo de salida en modo escritura.
        with open(output_file, 'w') as le_file:
            # Escribe el encabezado de protección contra múltiples inclusiones.
            le_file.write('#ifndef ENV_HPP\n')
            le_file.write('#define ENV_HPP\n\n')

            for key, value in env_vars.items():
                if value.isdigit():
                    # Si el valor es numérico, lo escribe tal cual sin comillas.
                    le_file.write(f'#define {key} {value}\n')

                else:
                    # Si el valor es una cadena, lo envuelve entre comillas.
                    le_file.write(f'#define {key} "{value}"\n')

            # Cierra el bloque de protección.
            le_file.write('\n#endif\n')

    @staticmethod
    def process(env_file, output_file):
        '''
        Ejecuta el proceso completo de carga de variables de entorno
        y generación del archivo de encabezado.

        :param env_file: Ruta del archivo .env que contiene las variables de entorno.
        :param output_file: Ruta del archivo de salida que contendrá las definiciones.
        '''

        # Carga las variables de entorno desde el archivo .env.
        env_vars = EnvVarManager.load_dotenv(env_file)
        # Genera el archivo de encabezado.
        EnvVarManager.generate_header_file(env_vars, output_file)

if __name__ == '__main__':
    '''
    Ejecuta el proceso completo usando un archivo .env
    y generando un archivo env.hpp en la carpeta src.
    '''
    EnvVarManager.process('.env', 'src/env.hpp')