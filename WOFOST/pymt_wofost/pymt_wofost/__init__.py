#! /usr/bin/env python
import pkg_resources

__version__ = pkg_resources.get_distribution("pymt_Mags_rf").version


from .bmi import WOFOST_MaizeTW

__all__ = [
    "WOFOST_MaizeTW",
]
