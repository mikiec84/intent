from ..strutil import *

def test_truncate_if_needed():
    hw = "hello, world"
    assert truncate_if_needed(hw, 5) == "he..."
    assert truncate_if_needed(hw, 15) == hw