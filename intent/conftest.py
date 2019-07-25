import pytest
import tempfile


@pytest.fixture
def scratchdir():
    x = tempfile.TemporaryDirectory()
    yield x
    x.cleanup()