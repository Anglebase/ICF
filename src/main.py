from sys import argv

from PyQt6.QtWidgets import QWidget, QApplication

app = QApplication(argv)
window = QWidget()
window.show()
app.exec()
