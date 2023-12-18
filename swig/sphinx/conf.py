# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html
import os
import sys

# -- Project information --------------------------------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'GNSSTk'
copyright = '2022, ARL:UT'

import gnsstk
version = '14.3'
release = '14.3.0'

# -- General configuration ------------------------------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

# Starting the list of sphinx extensions here. Add the extension below with each
# respective configuration section.
extensions = []

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# Remove the package/module prefix to each documented item
add_module_names = False

# -- ImgMath Options ------------------------------------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/extensions/math.html#module-sphinx.ext.imgmath
# The builtin ImgMath extension renders latex math blocks as images to be imbedded in html
extensions.append("sphinx.ext.imgmath")

# Using svg, instead of png, so that the math can scale with the page
imgmath_image_format="svg"

# Embed the images into the html. Seemed to be easier.
imgmath_embed=True

# Make the math stand out more
imgmath_font_size=14

# -- Napoleon Options -----------------------------------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/extensions/napoleon.html#configuration
# The builtin napoleon extension allows for parsing numpy and google style docstrings into
# reStructured text. TKS should follow google style docstrings, but numpy style is not explicitly
# excluded here.
extensions.append("sphinx.ext.napoleon")

# -- Autodoc Options ------------------------------------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/extensions/autodoc.html#configuration
# The builtin AutoDoc extension offers reST directives for automatically extracting docstrings
# from various python objects.
extensions.append("sphinx.ext.autodoc")

# Only insert the class docstring for class documentation.
# This is opposed to also including the __init__ documentation.
# Requires documenting the __init__ as a separate method which we do in the autosummary template
# for classes
autoclass_content = "class"

# Separates class "signature" from the class documentation.
# i.e. it creates a separate method doc for the special __new__ method
autodoc_class_signature = "separated"

# Instead of evaluating signature defaults, render the original source code.
# For instance, given a global constant `THE_ANSWER=42`, if the variable is used as a
# default for parameter (e.g. `def func(a=THE_ANSWER)`) then the documentation will
# show the default as the variable name, not the actual value
autodoc_preserve_defaults = True

# -- Autosummary Options --------------------------------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/extensions/autosummary.html
# Autosummary is a built-in extension that creates summary lists
extensions.append("sphinx.ext.autosummary")

# -- Copy Button Options --------------------------------------------------------------------------
# https://sphinx-copybutton.readthedocs.io/en/latest/use.html
# The copy button extension provides easy to use button for copying example code from the
# documentation.
extensions.append("sphinx_copybutton")

# The copy button can strip out input prompts
# This regex identifies input prompts of python REPL style, typical bash style, and IPython style
copybutton_prompt_text = r">>> |\.\.\. |\$ |In \[\d*\]: | {2,5}\.\.\.: | {5,8}: "
copybutton_prompt_is_regexp = True

# -- Options for HTML output ----------------------------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output
# For the PyData theme see
# https://pydata-sphinx-theme.readthedocs.io/en/stable/

# Using the pydata theme.
html_theme = 'pydata_sphinx_theme'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# Paths here are relative to `html_static_path`
html_css_files = [
    'css/custom.css',
]

