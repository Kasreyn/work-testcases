const pineapple = document.getElementById('badPineapple');

pineapple.addEventListener('mouseover', () => {
  const maxX = window.innerWidth - pineapple.offsetWidth;
  const maxY = window.innerHeight - pineapple.offsetHeight;
  
  const newX = Math.random() * maxX;
  const newY = Math.random() * maxY;

  pineapple.style.left = newX + 'px';
  pineapple.style.top = newY + 'px';
});
