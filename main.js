const express = require('express'); 
const { readFile } = require('fs').promises;

const app = express();
app.use(express.static('public'));
app.use(express.urlencoded({extended: true}));

app.get('/', async (req, res) => {
    res.send(await readFile('./main.html', 'utf8'));
});

app.post('/', (req, res) => {
    console.log(req.body);
    res.redirect('/');
});

app.listen(process.env.PORT || 3000, () => console.log(`http://localhost:3000`));