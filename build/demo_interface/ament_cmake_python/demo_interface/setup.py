from setuptools import find_packages
from setuptools import setup

setup(
    name='demo_interface',
    version='0.0.0',
    packages=find_packages(
        include=('demo_interface', 'demo_interface.*')),
)
