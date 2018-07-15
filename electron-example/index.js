const { app, BrowserWindow, Menu, ipcMain } = require('electron')

const path = require('path')
const url = require('url')



ipcMain.on(
  'submit',
  (event, numberOfPoints, radius) => {
    win.webContents.send('paramsChanged', numberOfPoints, radius)
    if (dialogWin) {
      dialogWin.close()
    }
  })
  
const template = [{
  label: 'File',
  submenu: [{
    label: "Set parameters",
    click: createDialog
  },
    {
      role: 'quit'
    }
  ]
}, {
  label: 'View',
  submenu: [{
      role: 'reload'
    },
    {
      role: 'toggledevtools'
    }
  ]
}]


const menu = Menu.buildFromTemplate(template)
Menu.setApplicationMenu(menu)

let win
let dialogWin = null

function createWindow() {
  win = new BrowserWindow({
    width: 800,
    height: 600
  })

  win.loadURL(url.format({
    pathname: path.join(__dirname, 'index.html'),
    protocol: 'file:',
    slashes: true
  }))

  win.on('closed', () => {
    win = null
  })
}

function createDialog() {
  if (dialogWin != null) {
    return
  }
  dialogWin = new BrowserWindow({
    parent: win,
    width: 400,
    height: 300
  })

  dialogWin.loadURL(url.format({
    pathname: path.join(__dirname, 'dialog.html'),
    protocol: 'file:',
    slashes: true
  }))

  dialogWin.on('closed', () => {
    dialogWin = null
  })
}


app.on('ready', createWindow)

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

app.on('activate', () => {
  if (win === null) {
    createWindow()
  }
})
