function getAssetUrl(path) {
    if (window.embeddedAssets && window.embeddedAssets[path]) {
        return window.embeddedAssets[path];
    }
    return path;
}

const faces = [
    { id: 'normal', label: 'Normal' },
    { id: 'happy', label: 'Alegria' },
    { id: 'sad', label: 'Tristeza' },
    { id: 'angry', label: 'Raiva' },
    { id: 'sleepy', label: 'Com Sono' },
    { id: 'surprised', label: 'Surpresa' },
    { id: 'winking', label: 'Piscando' },
    { id: 'dead', label: 'Erro/Morto' },
    { id: 'suspicious', label: 'Suspeito' },
    { id: 'scared', label: 'Medo' },
    { id: 'love', label: 'Apaixonado' },
    { id: 'dizzy', label: 'Tonto' },
    { id: 'thinking', label: 'Pensando' },
    { id: 'focused', label: 'Focado' },
    { id: 'loading', label: 'Carregando' },
    { id: 'photo', label: 'Foto' },
    { id: 'disgust', label: 'Nojo' },
    { id: 'contempt', label: 'Desprezo' },
    { id: 'music', label: 'Música' },
    { id: 'battery', label: 'Bateria Fraca' },
    { id: 'cool', label: 'Estiloso' },
    { id: 'stars', label: 'Estrelas' },
    { id: 'cute', label: 'Fofo' },
    { id: 'sick', label: 'Doente' },
    { id: 'idea', label: 'Ideia' },
    { id: 'confused', label: 'Confuso' },
    { id: 'hot', label: 'Calor' },
    { id: 'crying', label: 'Chorando' },
    { id: 'cat', label: 'Gatinho' },
    { id: 'silly', label: 'Bobo' },
    { id: 'smile', label: 'Sorridente' },
    { id: 'shocked', label: 'Chocado' },
    { id: 'whistle', label: 'Assobiando' },
    { id: 'raised-brow', label: 'Sobrancelha' },
    { id: 'furious', label: 'Furioso' },
    { id: 'worried', label: 'Preocupado' },
    { id: 'tense', label: 'Tenso' },
    { id: 'angel', label: 'Anjinho' },
    { id: 'star-eyes', label: 'Fascinado' },
    { id: 'zipper', label: 'Zíper' },
    { id: 'like', label: 'Like' },
    { id: 'dislike', label: 'Não Like' },
    { id: 'sleep', label: 'Dormindo' },
    { id: 'money', label: 'Dinheiro' },
    { id: 'kiss', label: 'Beijo' },
    { id: 'alert', label: 'Alerta' },
    { id: 'celebrate', label: 'Festa' },
    { id: 'fire', label: 'Fogo' },
    { id: 'bug', label: 'Erro/Bug' },
    { id: 'game', label: 'Jogando' },
    { id: 'love2', label: 'Apaixonado 2' },
    { id: 'dead2', label: 'Morto 2' },
    { id: 'ghost', label: 'Fantasma' },
    { id: 'rocket', label: 'Turbo' },
    { id: 'robot', label: 'Robô' },
    { id: 'star2', label: 'Estrela 2' },
    { id: 'cool2', label: 'Estiloso 2' },
    { id: 'scared3', label: 'Medo 3' },
    { id: 'sleepy3', label: 'Sono 3' },
    { id: 'happy3', label: 'Alegria 3' },
    { id: 'angry2', label: 'Raiva 2' },
    { id: 'alien', label: 'Alienígena' },
    { id: 'funny2', label: 'Bobo 2' },
    { id: 'heart2', label: 'Coração 2' },
    { id: 'sleepy4', label: 'Sono 4' },
    { id: 'happy4', label: 'Alegria 4' },
    { id: 'devil', label: 'Travesso' },
    { id: 'angel2', label: 'Anjinho 2' },
    { id: 'rainbow', label: 'Arco-Íris' },
    { id: 'clown', label: 'Palhaço' },
    { id: 'music2', label: 'Música 2' },
    { id: 'scared4', label: 'Perigo' },
    { id: 'magic', label: 'Mágica' },
    { id: 'money3', label: 'Dinheiro 2' },
    { id: 'ninja', label: 'Ninja' },
    { id: 'buddy-normal', label: 'Buddy Normal', hasPupils: true },
    { id: 'buddy-happy', label: 'Buddy Alegre', hasPupils: true },
    { id: 'buddy-sad', label: 'Buddy Triste', hasPupils: true },
    { id: 'buddy-angry', label: 'Buddy Bravo', hasPupils: true },
    { id: 'buddy-suspicious', label: 'Buddy Desconfiado', hasPupils: true },
    // New Faces - CSS Expressions
    { id: 'new-01', label: 'Normal' },
    { id: 'new-02', label: 'Feliz' },
    { id: 'new-03', label: 'Muito Feliz' },
    { id: 'new-04', label: 'Piscando' },
    { id: 'new-05', label: 'Rindo' },
    { id: 'new-06', label: 'Normal 2' },
    { id: 'new-07', label: 'Beijando' },
    { id: 'new-08', label: 'Estrelado' },
    { id: 'new-10', label: 'Brilhante' },
    { id: 'new-11', label: 'Chateado' },
    { id: 'new-12', label: 'Dentuço' },
    { id: 'new-13', label: 'Apaixonado' },
    { id: 'new-14', label: 'Espirrando' },
    { id: 'new-15', label: 'Bravo' },
    { id: 'new-16', label: 'Surpreso O' },
    { id: 'new-17', label: 'Preocupado' },
    { id: 'new-18', label: 'Bigode' },
    { id: 'new-19', label: 'Bigode 2' },
    { id: 'new-21', label: 'Chorando' },
    { id: 'new-22', label: 'Surpreso' },
    { id: 'new-23', label: 'Nervoso' },
    { id: 'new-24', label: 'Feliz Grande' },
    { id: 'new-25', label: 'Tonto' },
    { id: 'new-26', label: 'Suando' },
    { id: 'new-27', label: 'Triste' },
    { id: 'new-28', label: 'Irritado' },
    { id: 'new-30', label: 'Sorrindo' },
    { id: 'new-31', label: 'Feliz' },
    { id: 'new-32', label: 'Malvado' },
    { id: 'new-33', label: 'Triste Lágrima' },
    { id: 'new-34', label: 'Uau' },
    { id: 'new-35', label: 'Óculos' },
    { id: 'new-37', label: 'Bravo' },
    { id: 'new-38', label: 'Muito Bravo' },
    { id: 'new-39', label: 'Malvado O' },
    { id: 'new-40', label: 'Tonto Espiral' },
    { id: 'new-41', label: 'Coração Partido' },
    { id: 'new-42', label: 'Dormindo' },
    { id: 'new-43', label: 'Bigode Bravo' },
    { id: 'new-44', label: 'Surpreso Pequeno' },
    { id: 'new-45', label: 'Nervoso Olhos' },
    { id: 'new-47', label: 'Feliz Ondulado' },
    { id: 'new-48', label: 'Malvado Surpreso' },
    { id: 'new-49', label: 'Bravo O' },
    { id: 'new-50', label: 'Bravo O 2' },
    { id: 'new-51', label: 'Surpreso Triste' },
    { id: 'new-52', label: 'Bigode Triste' },
    { id: 'new-53', label: 'Chocado' },
    { id: 'new-54', label: 'Triste' },
    { id: 'new-55', label: 'Chorando' },
    { id: 'new-57', label: 'Morto X' },
    { id: 'new-58', label: 'Chorando Espiral' },
    { id: 'new-59', label: 'Bravo Bigode' },
    { id: 'new-60', label: 'Bravo Bigode 2' }
];

const faceScreen = document.querySelector('.face-screen');
const controlsContainer = document.getElementById('controls');

// Tab and Viewport Elements
const tabVector = document.getElementById('tab-vector');
const tabImage = document.getElementById('tab-image');
const tabSvg = document.getElementById('tab-svg');
const panelVector = document.getElementById('panel-vector');
const panelImage = document.getElementById('panel-image');
const panelSvg = document.getElementById('panel-svg');
const customSectionVector = document.getElementById('custom-section-vector');
const customSectionImage = document.getElementById('custom-section-image');
const customSectionSvg = document.getElementById('custom-section-svg');
const faceWrapper = document.querySelector('.face-wrapper');
const imageFaceContainer = document.querySelector('.image-face-container');
const imageFaceFrame = document.getElementById('image-face-frame');
const imageControlsContainer = document.getElementById('image-controls');
const svgFaceContainer = document.querySelector('.svg-face-container');
const svgFaceImage = document.getElementById('svg-face-image');
const svgControlsContainer = document.getElementById('svg-controls');

// Image Customization Inputs
const sliderFps = document.getElementById('slider-fps');
const fpsValue = document.getElementById('fps-value');
const sliderHue = document.getElementById('slider-hue');
const sliderScale = document.getElementById('slider-scale');
const scaleValue = document.getElementById('scale-value');
const imageColorContainer = document.getElementById('image-color-controls');

// SVG Customization Inputs
const sliderSvgHue = document.getElementById('slider-svg-hue');
const sliderSvgScale = document.getElementById('slider-svg-scale');
const svgScaleValue = document.getElementById('svg-scale-value');
const svgColorContainer = document.getElementById('svg-color-controls');

// --- Web Audio API Setup para sons robóticos ---
const AudioContext = window.AudioContext || window.webkitAudioContext;
let audioCtx;

function initAudio() {
    if (!audioCtx) {
        audioCtx = new AudioContext();
    }
}

function playRoboticSound(faceId) {
    if (!audioCtx) return;

    const osc = audioCtx.createOscillator();
    const gainNode = audioCtx.createGain();

    osc.connect(gainNode);
    gainNode.connect(audioCtx.destination);

    const now = audioCtx.currentTime;

    // Parâmetros base padrão
    osc.type = 'square';
    gainNode.gain.setValueAtTime(0.05, now);

    switch (faceId) {
        case 'normal':
        case 'buddy-normal':
            osc.frequency.setValueAtTime(440, now);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.1);
            osc.start(now);
            osc.stop(now + 0.1);
            break;
        case 'happy':
        case 'buddy-happy':
            osc.frequency.setValueAtTime(523.25, now); // C5
            osc.frequency.setValueAtTime(659.25, now + 0.1); // E5
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'sad':
        case 'buddy-sad':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(349.23, now); // F4
            osc.frequency.exponentialRampToValueAtTime(200, now + 0.4);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'angry':
        case 'buddy-angry':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(100, now);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'sleepy':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(200, now);
            osc.frequency.linearRampToValueAtTime(100, now + 0.5);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.5);
            osc.start(now);
            osc.stop(now + 0.5);
            break;
        case 'surprised':
            osc.frequency.setValueAtTime(880, now); // A5
            gainNode.gain.setValueAtTime(0.1, now);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.15);
            osc.start(now);
            osc.stop(now + 0.15);
            break;
        case 'winking':
            osc.frequency.setValueAtTime(600, now);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.setValueAtTime(0.001, now + 0.05);
            gainNode.gain.setValueAtTime(0.05, now + 0.1);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.2);
            osc.start(now);
            osc.stop(now + 0.2);
            break;
        case 'dead':
            // pseudo-noise using sawtooth and rapid frequency modulation
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(100, now);
            osc.frequency.linearRampToValueAtTime(40, now + 0.3);
            gainNode.gain.setValueAtTime(0.1, now);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'suspicious':
        case 'buddy-suspicious':
            osc.frequency.setValueAtTime(300, now);
            osc.frequency.setValueAtTime(350, now + 0.15);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'scared':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(800, now);
            for (let i = 1; i < 10; i++) {
                osc.frequency.setValueAtTime(i % 2 == 0 ? 800 : 850, now + (i * 0.02));
            }
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.2);
            osc.start(now);
            osc.stop(now + 0.2);
            break;
        case 'love':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(440, now); // A4
            osc.frequency.setValueAtTime(554.37, now + 0.1); // C#5
            osc.frequency.setValueAtTime(659.25, now + 0.2); // E5
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.5);
            osc.start(now);
            osc.stop(now + 0.5);
            break;
        case 'dizzy':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(500, now);
            osc.frequency.linearRampToValueAtTime(800, now + 0.1);
            osc.frequency.linearRampToValueAtTime(400, now + 0.2);
            osc.frequency.linearRampToValueAtTime(700, now + 0.3);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'thinking':
            osc.type = 'square';
            osc.frequency.setValueAtTime(1000, now);
            gainNode.gain.setValueAtTime(0.02, now);
            gainNode.gain.setValueAtTime(0, now + 0.05);
            gainNode.gain.setValueAtTime(0.02, now + 0.1);
            gainNode.gain.setValueAtTime(0, now + 0.15);
            gainNode.gain.setValueAtTime(0.02, now + 0.2);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.25);
            osc.start(now);
            osc.stop(now + 0.25);
            break;
        case 'focused':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(200, now);
            osc.frequency.setValueAtTime(220, now + 0.2);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'loading':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(300, now);
            osc.frequency.setValueAtTime(400, now + 0.1);
            osc.frequency.setValueAtTime(300, now + 0.2);
            osc.frequency.setValueAtTime(400, now + 0.3);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.5);
            osc.start(now);
            osc.stop(now + 0.5);
            break;
        case 'photo':
            osc.type = 'square';
            osc.frequency.setValueAtTime(100, now);
            osc.frequency.exponentialRampToValueAtTime(8000, now + 0.1);
            gainNode.gain.setValueAtTime(0.1, now);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.15);
            osc.start(now);
            osc.stop(now + 0.15);
            break;
        case 'like':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(987.77, now); // B5
            osc.frequency.setValueAtTime(1318.51, now + 0.1); // E6
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'dislike':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(150, now);
            osc.frequency.setValueAtTime(140, now + 0.1);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'disgust':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(300, now);
            osc.frequency.exponentialRampToValueAtTime(100, now + 0.3);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'contempt':
            osc.type = 'square';
            osc.frequency.setValueAtTime(200, now);
            gainNode.gain.setValueAtTime(0.03, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.15);
            osc.start(now);
            osc.stop(now + 0.15);
            break;
        case 'music':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(440, now);
            osc.frequency.setValueAtTime(554.37, now + 0.1);
            osc.frequency.setValueAtTime(659.25, now + 0.2);
            osc.frequency.setValueAtTime(880, now + 0.3);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.5);
            osc.start(now);
            osc.stop(now + 0.5);
            break;
        case 'battery':
            osc.type = 'square';
            osc.frequency.setValueAtTime(150, now);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.setValueAtTime(0, now + 0.1);
            gainNode.gain.setValueAtTime(0.05, now + 0.2);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'cool':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(400, now);
            osc.frequency.exponentialRampToValueAtTime(50, now + 0.4);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'stars':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(1200, now);
            osc.frequency.setValueAtTime(1600, now + 0.05);
            osc.frequency.setValueAtTime(2000, now + 0.1);
            osc.frequency.setValueAtTime(1200, now + 0.15);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'cute':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(400, now);
            osc.frequency.exponentialRampToValueAtTime(800, now + 0.2);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'sick':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(100, now);
            osc.frequency.linearRampToValueAtTime(80, now + 0.2);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.1);
            gainNode.gain.setValueAtTime(0.05, now + 0.15);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'idea':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(1046.50, now); // C6
            gainNode.gain.setValueAtTime(0.1, now);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.5);
            osc.start(now);
            osc.stop(now + 0.5);
            break;
        case 'confused':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(300, now);
            osc.frequency.exponentialRampToValueAtTime(600, now + 0.3);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'hot':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(800, now);
            osc.frequency.linearRampToValueAtTime(200, now + 0.4);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'crying':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(600, now);
            osc.frequency.exponentialRampToValueAtTime(300, now + 0.5);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.6);
            osc.start(now);
            osc.stop(now + 0.6);
            break;
        case 'cat':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(800, now);
            osc.frequency.exponentialRampToValueAtTime(400, now + 0.4);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'silly':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(400, now);
            osc.frequency.exponentialRampToValueAtTime(800, now + 0.1);
            osc.frequency.exponentialRampToValueAtTime(300, now + 0.3);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'smile':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(523.25, now); // C5
            osc.frequency.setValueAtTime(659.25, now + 0.1); // E5
            osc.frequency.setValueAtTime(783.99, now + 0.2); // G5
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'shocked':
            osc.type = 'square';
            osc.frequency.setValueAtTime(200, now);
            osc.frequency.exponentialRampToValueAtTime(50, now + 0.3);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'whistle':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(1200, now);
            osc.frequency.linearRampToValueAtTime(1400, now + 0.2);
            osc.frequency.linearRampToValueAtTime(1000, now + 0.5);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.5);
            osc.start(now);
            osc.stop(now + 0.5);
            break;
        case 'raised-brow':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(100, now);
            osc.frequency.linearRampToValueAtTime(300, now + 0.3);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'furious':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(50, now); // Rosnado grave
            osc.frequency.linearRampToValueAtTime(60, now + 0.5);
            gainNode.gain.setValueAtTime(0.1, now);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.6);
            osc.start(now);
            osc.stop(now + 0.6);
            break;
        case 'worried':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(800, now);
            osc.frequency.exponentialRampToValueAtTime(700, now + 0.4);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.001, now + 0.5);
            osc.start(now);
            osc.stop(now + 0.5);
            break;
        case 'tense':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(800, now);
            osc.frequency.linearRampToValueAtTime(850, now + 0.1);
            osc.frequency.linearRampToValueAtTime(800, now + 0.2);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'angel':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(523.25, now); // C5
            const osc2 = audioCtx.createOscillator();
            osc2.type = 'sine';
            osc2.frequency.setValueAtTime(659.25, now); // E5
            osc2.connect(gainNode);
            osc2.start(now);
            osc2.stop(now + 1.5);
            gainNode.gain.setValueAtTime(0, now);
            gainNode.gain.linearRampToValueAtTime(0.05, now + 0.5);
            gainNode.gain.linearRampToValueAtTime(0, now + 1.5);
            osc.start(now);
            osc.stop(now + 1.5);
            break;
        case 'star-eyes':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(800, now);
            osc.frequency.setValueAtTime(1200, now + 0.1);
            osc.frequency.setValueAtTime(1600, now + 0.2);
            osc.frequency.setValueAtTime(2000, now + 0.3);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.6);
            osc.start(now);
            osc.stop(now + 0.6);
            break;
        case 'zipper':
            osc.type = 'square';
            osc.frequency.setValueAtTime(100, now);
            osc.frequency.linearRampToValueAtTime(400, now + 0.2);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.2);
            osc.start(now);
            osc.stop(now + 0.2);
            break;
        case 'like':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(800, now);
            osc.frequency.setValueAtTime(1200, now + 0.1);
            gainNode.gain.setValueAtTime(0.1, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'dislike':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(300, now);
            osc.frequency.linearRampToValueAtTime(150, now + 0.3);
            gainNode.gain.setValueAtTime(0.1, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'sleep':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(200, now);
            osc.frequency.linearRampToValueAtTime(150, now + 1.5);
            osc.frequency.linearRampToValueAtTime(200, now + 3);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0.02, now + 1.5);
            gainNode.gain.linearRampToValueAtTime(0.05, now + 3);
            osc.start(now);
            osc.stop(now + 3);
            break;
        case 'money':
            osc.type = 'square';
            osc.frequency.setValueAtTime(800, now);
            osc.frequency.setValueAtTime(1200, now + 0.1);
            osc.frequency.setValueAtTime(1600, now + 0.2);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'kiss':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(1000, now);
            osc.frequency.exponentialRampToValueAtTime(1500, now + 0.2);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.2);
            osc.start(now);
            osc.stop(now + 0.2);
            break;
        case 'alert':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(600, now);
            osc.frequency.linearRampToValueAtTime(800, now + 0.15);
            osc.frequency.linearRampToValueAtTime(600, now + 0.3);
            gainNode.gain.setValueAtTime(0.08, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'celebrate':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(523.25, now); // C5
            osc.frequency.setValueAtTime(659.25, now + 0.08); // E5
            osc.frequency.setValueAtTime(783.99, now + 0.16); // G5
            osc.frequency.setValueAtTime(1046.50, now + 0.24); // C6
            gainNode.gain.setValueAtTime(0.06, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'fire':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(80, now);
            osc.frequency.linearRampToValueAtTime(400, now + 0.3);
            gainNode.gain.setValueAtTime(0.1, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'bug':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(120, now);
            osc.frequency.linearRampToValueAtTime(80, now + 0.25);
            gainNode.gain.setValueAtTime(0.1, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.25);
            osc.start(now);
            osc.stop(now + 0.25);
            break;
        case 'game':
            osc.type = 'square';
            osc.frequency.setValueAtTime(300, now);
            osc.frequency.setValueAtTime(600, now + 0.1);
            gainNode.gain.setValueAtTime(0.06, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.25);
            osc.start(now);
            osc.stop(now + 0.25);
            break;
        case 'love2':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(600, now);
            osc.frequency.exponentialRampToValueAtTime(1200, now + 0.4);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'dead2':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(200, now);
            osc.frequency.linearRampToValueAtTime(50, now + 0.5);
            gainNode.gain.setValueAtTime(0.08, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.5);
            osc.start(now);
            osc.stop(now + 0.5);
            break;
        case 'ghost':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(400, now);
            osc.frequency.linearRampToValueAtTime(600, now + 0.2);
            osc.frequency.linearRampToValueAtTime(400, now + 0.4);
            osc.frequency.linearRampToValueAtTime(600, now + 0.6);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.6);
            osc.start(now);
            osc.stop(now + 0.6);
            break;
        case 'rocket':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(100, now);
            osc.frequency.exponentialRampToValueAtTime(2000, now + 0.8);
            gainNode.gain.setValueAtTime(0.1, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.8);
            osc.start(now);
            osc.stop(now + 0.8);
            break;
        case 'robot':
            osc.type = 'square';
            osc.frequency.setValueAtTime(440, now);
            osc.frequency.setValueAtTime(220, now + 0.1);
            osc.frequency.setValueAtTime(880, now + 0.2);
            gainNode.gain.setValueAtTime(0.06, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'star2':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(987.77, now); // B5
            osc.frequency.setValueAtTime(1318.51, now + 0.1); // E6
            gainNode.gain.setValueAtTime(0.04, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'cool2':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(150, now);
            osc.frequency.linearRampToValueAtTime(300, now + 0.5);
            gainNode.gain.setValueAtTime(0.08, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.5);
            osc.start(now);
            osc.stop(now + 0.5);
            break;
        case 'scared3':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(900, now);
            osc.frequency.linearRampToValueAtTime(1200, now + 0.1);
            osc.frequency.linearRampToValueAtTime(900, now + 0.2);
            osc.frequency.linearRampToValueAtTime(1200, now + 0.3);
            gainNode.gain.setValueAtTime(0.08, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'sleepy3':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(150, now);
            gainNode.gain.setValueAtTime(0, now);
            gainNode.gain.linearRampToValueAtTime(0.05, now + 0.6);
            gainNode.gain.linearRampToValueAtTime(0, now + 1.2);
            osc.start(now);
            osc.stop(now + 1.2);
            break;
        case 'happy3':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(400, now);
            osc.frequency.exponentialRampToValueAtTime(1200, now + 0.4);
            gainNode.gain.setValueAtTime(0.06, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'angry2':
            osc.type = 'square';
            osc.frequency.setValueAtTime(180, now);
            osc.frequency.setValueAtTime(220, now + 0.05);
            osc.frequency.setValueAtTime(180, now + 0.1);
            osc.frequency.setValueAtTime(220, now + 0.15);
            osc.frequency.setValueAtTime(180, now + 0.2);
            gainNode.gain.setValueAtTime(0.08, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.25);
            osc.start(now);
            osc.stop(now + 0.25);
            break;
        case 'alien':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(800, now);
            osc.frequency.exponentialRampToValueAtTime(300, now + 0.6);
            // Vibrato
            const fm = audioCtx.createOscillator();
            fm.frequency.value = 15; // 15Hz vibrato
            const fmGain = audioCtx.createGain();
            fmGain.gain.value = 100;
            fm.connect(fmGain);
            fmGain.connect(osc.frequency);
            fm.start(now);
            fm.stop(now + 0.6);
            
            gainNode.gain.setValueAtTime(0.06, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.6);
            osc.start(now);
            osc.stop(now + 0.6);
            break;
        case 'funny2':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(200, now);
            osc.frequency.exponentialRampToValueAtTime(800, now + 0.3);
            gainNode.gain.setValueAtTime(0.08, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'heart2':
            osc.type = 'sine';
            // Batida 1
            osc.frequency.setValueAtTime(60, now);
            gainNode.gain.setValueAtTime(0.15, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.1);
            // Batida 2
            const osc_b = audioCtx.createOscillator();
            osc_b.type = 'sine';
            osc_b.frequency.setValueAtTime(60, now + 0.25);
            const gain_b = audioCtx.createGain();
            osc_b.connect(gain_b);
            gain_b.connect(audioCtx.destination);
            gain_b.gain.setValueAtTime(0, now);
            gain_b.gain.setValueAtTime(0.15, now + 0.25);
            gain_b.gain.linearRampToValueAtTime(0, now + 0.35);
            
            osc.start(now);
            osc.stop(now + 0.1);
            osc_b.start(now + 0.25);
            osc_b.stop(now + 0.35);
            break;
        case 'sleepy4':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(400, now);
            osc.frequency.linearRampToValueAtTime(200, now + 0.8);
            gainNode.gain.setValueAtTime(0.04, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.8);
            osc.start(now);
            osc.stop(now + 0.8);
            break;
        case 'happy4':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(600, now);
            osc.frequency.exponentialRampToValueAtTime(1800, now + 0.3);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'devil':
            osc.type = 'square';
            osc.frequency.setValueAtTime(150, now);
            osc.frequency.setValueAtTime(180, now + 0.15);
            osc.frequency.setValueAtTime(130, now + 0.3);
            gainNode.gain.setValueAtTime(0.08, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.45);
            osc.start(now);
            osc.stop(now + 0.45);
            break;
        case 'angel2':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(880, now); // A5
            const osc_c = audioCtx.createOscillator();
            osc_c.type = 'sine';
            osc_c.frequency.setValueAtTime(1100, now); // C#6
            osc_c.connect(gainNode);
            osc_c.start(now);
            osc_c.stop(now + 0.6);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.6);
            osc.start(now);
            osc.stop(now + 0.6);
            break;
        case 'rainbow':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(200, now);
            osc.frequency.linearRampToValueAtTime(800, now + 0.3);
            osc.frequency.linearRampToValueAtTime(200, now + 0.6);
            gainNode.gain.setValueAtTime(0.07, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.6);
            osc.start(now);
            osc.stop(now + 0.6);
            break;
        case 'clown':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(500, now);
            osc.frequency.setValueAtTime(250, now + 0.08);
            osc.frequency.setValueAtTime(500, now + 0.16);
            gainNode.gain.setValueAtTime(0.1, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'music2':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(523.25, now); // C5
            gainNode.gain.setValueAtTime(0.06, now);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.5);
            osc.start(now);
            osc.stop(now + 0.5);
            break;
        case 'scared4':
            osc.type = 'sawtooth';
            osc.frequency.setValueAtTime(180, now);
            osc.frequency.setValueAtTime(100, now + 0.15);
            gainNode.gain.setValueAtTime(0.1, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.3);
            osc.start(now);
            osc.stop(now + 0.3);
            break;
        case 'magic':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(1000, now);
            osc.frequency.setValueAtTime(1500, now + 0.08);
            osc.frequency.setValueAtTime(2000, now + 0.16);
            osc.frequency.setValueAtTime(2500, now + 0.24);
            gainNode.gain.setValueAtTime(0.04, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'money3':
            osc.type = 'square';
            osc.frequency.setValueAtTime(987.77, now); // B5
            osc.frequency.setValueAtTime(1318.51, now + 0.08); // E6
            gainNode.gain.setValueAtTime(0.06, now);
            gainNode.gain.linearRampToValueAtTime(0.02, now + 0.15);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.4);
            osc.start(now);
            osc.stop(now + 0.4);
            break;
        case 'ninja':
            osc.type = 'triangle';
            osc.frequency.setValueAtTime(600, now);
            osc.frequency.exponentialRampToValueAtTime(100, now + 0.25);
            gainNode.gain.setValueAtTime(0.08, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.25);
            osc.start(now);
            osc.stop(now + 0.25);
            break;
        case 'new-01':
        case 'new-02':
        case 'new-03':
        case 'new-04':
        case 'new-05':
        case 'new-06':
        case 'new-07':
        case 'new-08':
        case 'new-10':
        case 'new-11':
        case 'new-12':
        case 'new-13':
        case 'new-14':
        case 'new-15':
        case 'new-16':
        case 'new-17':
        case 'new-18':
        case 'new-19':
        case 'new-21':
        case 'new-22':
        case 'new-23':
        case 'new-24':
        case 'new-25':
        case 'new-26':
        case 'new-27':
        case 'new-28':
        case 'new-30':
        case 'new-31':
        case 'new-32':
        case 'new-33':
        case 'new-34':
        case 'new-35':
        case 'new-37':
        case 'new-38':
        case 'new-39':
        case 'new-40':
        case 'new-41':
        case 'new-42':
        case 'new-43':
        case 'new-44':
        case 'new-45':
        case 'new-47':
        case 'new-48':
        case 'new-49':
        case 'new-50':
        case 'new-51':
        case 'new-52':
        case 'new-53':
        case 'new-54':
        case 'new-55':
        case 'new-57':
        case 'new-58':
        case 'new-59':
        case 'new-60':
            osc.type = 'sine';
            osc.frequency.setValueAtTime(600, now);
            osc.frequency.exponentialRampToValueAtTime(800, now + 0.1);
            gainNode.gain.setValueAtTime(0.05, now);
            gainNode.gain.linearRampToValueAtTime(0, now + 0.2);
            osc.start(now);
            osc.stop(now + 0.2);
            break;
        default:
            osc.frequency.setValueAtTime(400, now);
            gainNode.gain.exponentialRampToValueAtTime(0.001, now + 0.1);
            osc.start(now);
            osc.stop(now + 0.1);
    }
}

// Initialize UI
let activeFaceId = 'normal';
let normalModeTimeoutId = null;

function startNormalModeVariation() {
    stopNormalModeVariation();

    function tick() {
        if (activeFaceId !== 'normal') return;

        const possibleFaces = ['normal', 'happy', 'surprised', 'winking', 'thinking'];
        const currentFace = possibleFaces.find(f => faceScreen.classList.contains(f)) || 'normal';
        
        let nextFace;
        do {
            nextFace = possibleFaces[Math.floor(Math.random() * possibleFaces.length)];
        } while (nextFace === currentFace);

        // Remove all face classes
        faceScreen.className = 'face-screen';
        
        // Trigger reflow
        void faceScreen.offsetWidth;

        // Add new face class
        faceScreen.classList.add(nextFace);

        // Play sound if audio context is active
        if (audioCtx && audioCtx.state === 'running') {
            playRoboticSound(nextFace);
        }

        // Schedule next transition in 15 to 30 seconds
        const nextDelay = Math.random() * 15000 + 15000;
        normalModeTimeoutId = setTimeout(tick, nextDelay);
    }

    // First transition in 15 to 30 seconds
    const firstDelay = Math.random() * 15000 + 15000;
    normalModeTimeoutId = setTimeout(tick, firstDelay);
}

function stopNormalModeVariation() {
    if (normalModeTimeoutId) {
        clearTimeout(normalModeTimeoutId);
        normalModeTimeoutId = null;
    }
}

// Mode selection state and configuration
let activeMode = 'vector'; // 'vector', 'image', or 'svg'

const imageExpressions = [
    { id: 'idle', label: 'Espera (Idle)', frames: 75, sound: 'normal' },
    { id: 'happy', label: 'Feliz', frames: 8, sound: 'happy' },
    { id: 'sad', label: 'Triste', frames: 8, sound: 'sad' },
    { id: 'angry', label: 'Raiva', frames: 8, sound: 'angry' },
    { id: 'bored', label: 'Entediado', frames: 8, sound: 'sleepy' },
    { id: 'confused', label: 'Confuso', frames: 6, sound: 'confused' },
    { id: 'curious', label: 'Curioso', frames: 8, sound: 'thinking' },
    { id: 'dizzy', label: 'Tonto', frames: 4, sound: 'dizzy' },
    { id: 'sleepy', label: 'Com Sono', frames: 8, sound: 'sleepy' },
    { id: 'surprised', label: 'Surpreso', frames: 4, sound: 'surprised' },
    { id: 'thinking', label: 'Pensando', frames: 8, sound: 'thinking' },
    { id: 'speaking', label: 'Falando', frames: 6, sound: 'whistle' },
    { id: 'listening', label: 'Ouvindo', frames: 36, sound: 'loading' },
    { id: 'low_battery', label: 'Bateria Baixa', frames: 8, sound: 'battery' },
    { id: 'error', label: 'Erro', frames: 4, sound: 'dead' },
    { id: 'capturing', label: 'Capturando', frames: 4, sound: 'photo' },
    { id: 'cheeky', label: 'Atrevido', frames: 8, sound: 'silly' },
    { id: 'daydream', label: 'Devaneio', frames: 12, sound: 'sleep' },
    { id: 'detective', label: 'Detetive', frames: 8, sound: 'focused' },
    { id: 'heart', label: 'Coração', frames: 6, sound: 'love' },
    { id: 'starry_eyed', label: 'Brilho nos Olhos', frames: 8, sound: 'stars' },
    { id: 'jamming', label: 'Curtindo Música', frames: 8, sound: 'music' },
    { id: 'sir_mano', label: 'Sir Mano', frames: 8, sound: 'normal' },
    { id: 'bee', label: 'Abelha', frames: 16, sound: 'alert' },
    { id: 'ladybug', label: 'Joaninha', frames: 16, sound: 'alert' },
    { id: 'worm', label: 'Minhoca', frames: 16, sound: 'game' },
    { id: 'football', label: 'Futebol', frames: 8, sound: 'celebrate' },
    { id: 'shhh', label: 'Silêncio (Shhh)', frames: 8, sound: 'zipper' },
    { id: 'warmup', label: 'Aquecimento', frames: 4, sound: 'normal' }
];

let currentImageExpressionId = 'idle';
let imageFrameIndex = 1;
let imageAnimTimeout = null;
let animationFps = 10;

const _preloadedExpr = {};
function preloadImageExpression(expr) {
    if (!expr || _preloadedExpr[expr.id]) return;
    const cache = [];
    for (let i = 1; i <= expr.frames; i++) {
        const pf = String(i).padStart(2, '0');
        const img = new Image();
        img.src = getAssetUrl(`../faces/${expr.id}/${expr.id}_${pf}.png`);
        cache.push(img);
    }
    _preloadedExpr[expr.id] = cache;
}
function startImageAnimation(expressionId) {
    stopImageAnimation();
    currentImageExpressionId = expressionId;
    imageFrameIndex = 1;
    
    const expr = imageExpressions.find(e => e.id === expressionId);
    if (!expr) return;
    preloadImageExpression(expr);

    function nextFrame() {
        if (activeMode !== 'image') return;

        const paddedFrame = String(imageFrameIndex).padStart(2, '0');
        imageFaceFrame.src = getAssetUrl(`../faces/${expr.id}/${expr.id}_${paddedFrame}.png`);

        imageFrameIndex++;
        if (imageFrameIndex > expr.frames) {
            imageFrameIndex = 1;
        }

        const interval = 1000 / animationFps;
        imageAnimTimeout = setTimeout(nextFrame, interval);
    }
    
    nextFrame();
}

function stopImageAnimation() {
    if (imageAnimTimeout) {
        clearTimeout(imageAnimTimeout);
        imageAnimTimeout = null;
    }
}

// Convert Hex to HSL for color calculation
function hexToHSL(hex) {
    let r = parseInt(hex.slice(1, 3), 16) / 255;
    let g = parseInt(hex.slice(3, 5), 16) / 255;
    let b = parseInt(hex.slice(5, 7), 16) / 255;

    let max = Math.max(r, g, b), min = Math.min(r, g, b);
    let h, s, l = (max + min) / 2;

    if (max === min) {
        h = s = 0; 
    } else {
        let d = max - min;
        s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
        switch (max) {
            case r: h = (g - b) / d + (g < b ? 6 : 0); break;
            case g: h = (b - r) / d + 2; break;
            case b: h = (r - g) / d + 4; break;
        }
        h /= 6;
    }

    return {
        h: Math.round(h * 360),
        s: Math.round(s * 100),
        l: Math.round(l * 100)
    };
}

// Apply Hue-Rotate/Grayscale Filter to recolor transparent cyan pngs
function applyImageFilter(hexColor) {
    if (hexColor.toLowerCase() === '#ffffff') {
        imageFaceFrame.style.filter = 'brightness(2) grayscale(1)';
    } else {
        const hsl = hexToHSL(hexColor);
        const targetHue = hsl.h;
        const baseHue = 180; // Cyan base color in pngs
        let hueRotate = targetHue - baseHue;
        if (hueRotate < 0) hueRotate += 360;
        
        imageFaceFrame.style.filter = `hue-rotate(${hueRotate}deg) saturate(1.3) brightness(1.2)`;
    }
}

// --- SVG Mode Setup ---
const svgFaces = [
    { id: 'happy', file: 'happy.svg', label: 'Feliz', sound: 'happy' },
    { id: 'happy_2', file: 'happy 2.svg', label: 'Feliz 2', sound: 'happy' },
    { id: 'smile', file: 'smile.svg', label: 'Sorriso', sound: 'happy' },
    { id: 'basic_ascii_smile', file: 'basic ASCII smile reaction.svg', label: 'ASCII Sorriso', sound: 'happy' },
    { id: 'ascii_open_smile', file: 'ASCII Open smile reaction.svg', label: 'ASCII Sorriso Aberto', sound: 'happy' },
    { id: 'frown', file: 'frown.svg', label: 'Triste', sound: 'sad' },
    { id: 'ascii_frown', file: 'ASCII frown reaction.svg', label: 'ASCII Triste', sound: 'sad' },
    { id: 'grumpy', file: 'grumpy.svg', label: 'Ranzinza', sound: 'angry' },
    { id: 'confused_mad', file: 'confused and mad.svg', label: 'Confuso/Bravo', sound: 'angry' },
    { id: 'cheeky', file: 'cheeky.svg', label: 'Irônico', sound: 'silly' },
    { id: 'crying', file: 'crying.svg', label: 'Chorando', sound: 'crying' },
    { id: 'dizzy', file: 'dizzy.svg', label: 'Tonto', sound: 'dizzy' },
    { id: 'exasperated', file: 'exasperated.svg', label: 'Exasperado', sound: 'worried' },
    { id: 'heart_eyes', file: 'heart eyes.svg', label: 'Coração', sound: 'cute' },
    { id: 'hmmm', file: 'hmmm.svg', label: 'Pensativo', sound: 'thinking' },
    { id: 'meep', file: 'meep.svg', label: 'Meep', sound: 'silly' },
    { id: 'ooooooh', file: 'ooooooh.svg', label: 'Ooh!', sound: 'surprised' },
    { id: 'open_mouth', file: 'open mouth.svg', label: 'Boca Aberta', sound: 'surprised' },
    { id: 'relax', file: 'relax.svg', label: 'Relaxado', sound: 'normal' },
    { id: 'shocked_wonder', file: 'shocked and wonder.svg', label: 'Chocado', sound: 'surprised' },
    { id: 'shouting', file: 'shouting.svg', label: 'Gritando', sound: 'furious' },
    { id: 'side_eye', file: 'side eye.svg', label: 'Desconfiado', sound: 'thinking' },
    { id: 'sigh', file: 'sigh.svg', label: 'Suspiro', sound: 'sad' },
    { id: 'star_eyes_1', file: 'stars eyes.svg', label: 'Estrelas 1', sound: 'star-eyes' },
    { id: 'star_eyes_2', file: 'star eyes 2.svg', label: 'Estrelas 2', sound: 'star-eyes' },
    { id: 'tired_happy', file: 'tired and happy.svg', label: 'Cansado/Feliz', sound: 'sleep' },
    { id: 'what', file: 'what!.svg', label: 'O quê?!', sound: 'confused' },
    { id: 'x_eyes', file: 'x eyes (dead).svg', label: 'Morto', sound: 'dizzy' },
    { id: 'bee', file: 'bee.svg', label: 'Abelha', sound: 'music' },
    { id: 'butterfly_1', file: 'butterfly-1.svg', label: 'Borboleta 1', sound: 'music' },
    { id: 'butterfly_2', file: 'butterfly.svg', label: 'Borboleta 2', sound: 'music' },
    { id: 'ladybug', file: 'ladybug.svg', label: 'Joaninha', sound: 'music' },
    { id: 'worm_1', file: 'worm-1.svg', label: 'Minhoca 1', sound: 'music' },
    { id: 'worm_2', file: 'worm.svg', label: 'Minhoca 2', sound: 'music' }
];

let currentSvgExpressionId = 'happy';

function startSvgFace(expressionId) {
    currentSvgExpressionId = expressionId;
    const face = svgFaces.find(f => f.id === expressionId);
    if (!face) return;
    svgFaceImage.src = getAssetUrl(`../svg_faces/${face.file}`);
}

function applySvgFilter(hexColor) {
    if (hexColor.toLowerCase() === '#ffffff') {
        svgFaceImage.style.filter = 'brightness(1.5) grayscale(1)';
    } else {
        const hsl = hexToHSL(hexColor);
        const baseHue = 180;
        let hueRotate = hsl.h - baseHue;
        if (hueRotate < 0) hueRotate += 360;
        svgFaceImage.style.filter = `hue-rotate(${hueRotate}deg)`;
    }
}


// Arduino-style Eye Colors Palette
const eyeColors = [
    { name: 'Violeta', hex: '#ff00ff' },
    { name: 'Ciano', hex: '#00ffff' },
    { name: 'Amarelo', hex: '#ffff00' },
    { name: 'Azul', hex: '#0066ff' },
    { name: 'Verde', hex: '#39ff14' },
    { name: 'Vermelho', hex: '#ff003c' },
    { name: 'Laranja', hex: '#ff9f00' },
    { name: 'Rosa', hex: '#ff66b2' },
    { name: 'Branco', hex: '#ffffff' }
];

// Helper to set eye color and dynamic glow (using RGB values for transparency)
function setEyeColor(hexColor) {
    document.documentElement.style.setProperty('--eye-color', hexColor);
    
    // Parse hex to rgb to update CSS custom property --eye-glow
    const r = parseInt(hexColor.slice(1, 3), 16);
    const g = parseInt(hexColor.slice(3, 5), 16);
    const b = parseInt(hexColor.slice(5, 7), 16);
    
    const glow = `0 0 20px rgba(${r}, ${g}, ${b}, 0.6), 0 0 40px rgba(${r}, ${g}, ${b}, 0.4)`;
    document.documentElement.style.setProperty('--eye-glow', glow);
}

// Generate Color Palette Controls
const colorContainer = document.getElementById('color-controls');
eyeColors.forEach((color, idx) => {
    const dot = document.createElement('div');
    dot.className = 'color-dot';
    dot.style.backgroundColor = color.hex;
    dot.title = color.name;
    dot.setAttribute('data-color', color.hex);
    
    // Default active color is Ciano (matches the original default var)
    if (idx === 1) dot.classList.add('active');
    
    dot.addEventListener('click', () => {
        document.querySelectorAll('.color-dot').forEach(d => d.classList.remove('active'));
        dot.classList.add('active');
        setEyeColor(color.hex);
        initAudio();
    });
    
    colorContainer.appendChild(dot);
});

// Shape Controls (Round vs Square)
const btnShapeRound = document.getElementById('btn-shape-round');
const btnShapeSquare = document.getElementById('btn-shape-square');

btnShapeRound.addEventListener('click', () => {
    initAudio();
    btnShapeRound.classList.add('active');
    btnShapeSquare.classList.remove('active');
    document.documentElement.style.setProperty('--eye-radius', '50%');
    document.documentElement.style.setProperty('--pupil-radius', '50%');
});

btnShapeSquare.addEventListener('click', () => {
    initAudio();
    btnShapeSquare.classList.add('active');
    btnShapeRound.classList.remove('active');
    document.documentElement.style.setProperty('--eye-radius', '15px');
    document.documentElement.style.setProperty('--pupil-radius', '6px');
});

// Pupil Wandering Animation (matching cos/sin logic from desk2.o.ino)
let pupilMovementEnabled = true;
let angle = 0;

function animatePupils() {
    if (pupilMovementEnabled) {
        angle += 0.02;
        if (angle > Math.PI * 2) angle -= Math.PI * 2;
        
        // Horizontal and vertical offsets computed exactly like Arduino
        const maxOffsetX = 20; // max horizontal movement
        const maxOffsetY = 15; // max vertical movement
        
        const offX = Math.cos(angle) * maxOffsetX;
        const offY = Math.sin(angle * 0.5) * maxOffsetY;
        
        document.documentElement.style.setProperty('--pupil-x', `${offX}px`);
        document.documentElement.style.setProperty('--pupil-y', `${offY}px`);
    } else {
        document.documentElement.style.setProperty('--pupil-x', `0px`);
        document.documentElement.style.setProperty('--pupil-y', `0px`);
    }
    
    requestAnimationFrame(animatePupils);
}
// Start pupil animation loop
animatePupils();

// Movement toggle controls
const btnMovementOn = document.getElementById('btn-movement-on');
const btnMovementOff = document.getElementById('btn-movement-off');

btnMovementOn.addEventListener('click', () => {
    initAudio();
    btnMovementOn.classList.add('active');
    btnMovementOff.classList.remove('active');
    pupilMovementEnabled = true;
});

btnMovementOff.addEventListener('click', () => {
    initAudio();
    btnMovementOff.classList.add('active');
    btnMovementOn.classList.remove('active');
    pupilMovementEnabled = false;
});

// Render Face Expressions Buttons
faceScreen.classList.add('normal');
startNormalModeVariation();

function assignUniqueLabels(list) {
    const used = new Set();
    list.forEach(item => {
        let label = item.label, n = 1;
        while (used.has(label)) { n++; label = item.label + ' (' + n + ')'; }
        used.add(label);
        item._btn = label;
    });
}
assignUniqueLabels(faces);
faces.forEach(face => {
    const btn = document.createElement('button');
    btn.textContent = face._btn || face.label; // Nome em português
    if (face.id === 'normal') btn.classList.add('active');

    btn.addEventListener('click', () => {
        // Init audio context on first user interaction (browser policy)
        initAudio();

        activeFaceId = face.id;

        // Remove active class from all buttons
        document.querySelectorAll('.button-grid button').forEach(b => b.classList.remove('active'));
        btn.classList.add('active');

        // Remove all face classes
        faceScreen.className = 'face-screen';

        // Trigger reflow to restart animations if needed
        void faceScreen.offsetWidth;

        // Add new face class
        faceScreen.classList.add(face.id);

        // Add has-pupils if applicable
        if (face.hasPupils) {
            faceScreen.classList.add('has-pupils');
        }

        // Play robotic sound
        playRoboticSound(face.id);

        if (face.id === 'normal') {
            startNormalModeVariation();
        } else {
            stopNormalModeVariation();
        }
    });

    controlsContainer.appendChild(btn);
});

// Blink logic
setInterval(() => {
    if (activeMode !== 'vector') return;
    // Check which face is currently active
    const currentFaceObj = faces.find(f => faceScreen.classList.contains(f.id));
    if (currentFaceObj && ['normal', 'happy', 'sad', 'surprised', 'thinking', 'dizzy', 'cute', 'music', 'confused', 'sick', 'hot', 'crying', 'cat', 'silly', 'smile', 'whistle', 'raised-brow', 'furious', 'worried', 'tense', 'angel', 'star-eyes', 'zipper', 'sleep', 'money', 'kiss', 'buddy-normal', 'buddy-happy', 'buddy-sad', 'buddy-angry', 'buddy-suspicious'].includes(currentFaceObj.id)) {
        const leftEye = document.querySelector('.left-eye');
        const rightEye = document.querySelector('.right-eye');

        leftEye.classList.add('blinking');
        rightEye.classList.add('blinking');
        leftEye.style.height = '10px';
        rightEye.style.height = '10px';

        setTimeout(() => {
            leftEye.style.height = '';
            rightEye.style.height = '';
            leftEye.classList.remove('blinking');
            rightEye.classList.remove('blinking');
        }, 150);
    }
}, Math.random() * 3000 + 3000);

// --- Tab Switching Logic (Helper Function) ---

function switchMode(newMode) {
    activeMode = newMode;
    
    // 1. Reset all tab buttons
    tabVector.classList.remove('active');
    tabImage.classList.remove('active');
    tabSvg.classList.remove('active');
    
    // 2. Hide all panels
    panelVector.style.display = 'none';
    panelImage.style.display = 'none';
    panelSvg.style.display = 'none';
    panelVector.classList.remove('active');
    panelImage.classList.remove('active');
    panelSvg.classList.remove('active');
    
    // 3. Hide all customization sections
    customSectionVector.style.display = 'none';
    customSectionImage.style.display = 'none';
    customSectionSvg.style.display = 'none';
    
    // 4. Hide all viewports
    faceWrapper.style.display = 'none';
    imageFaceContainer.style.display = 'none';
    svgFaceContainer.style.display = 'none';
    
    // 5. Active state specific setup
    if (newMode === 'vector') {
        tabVector.classList.add('active');
        panelVector.style.display = 'grid';
        panelVector.classList.add('active');
        customSectionVector.style.display = 'block';
        faceWrapper.style.display = 'flex';
        
        stopImageAnimation();
        if (activeFaceId === 'normal') {
            startNormalModeVariation();
        }
    } else if (newMode === 'image') {
        tabImage.classList.add('active');
        panelImage.style.display = 'grid';
        panelImage.classList.add('active');
        customSectionImage.style.display = 'block';
        imageFaceContainer.style.display = 'flex';
        
        stopNormalModeVariation();
        startImageAnimation(currentImageExpressionId);
    } else if (newMode === 'svg') {
        tabSvg.classList.add('active');
        panelSvg.style.display = 'grid';
        panelSvg.classList.add('active');
        customSectionSvg.style.display = 'block';
        svgFaceContainer.style.display = 'flex';
        
        stopImageAnimation();
        stopNormalModeVariation();
        startSvgFace(currentSvgExpressionId);
    }
}

tabVector.addEventListener('click', () => {
    initAudio();
    switchMode('vector');
});

tabImage.addEventListener('click', () => {
    initAudio();
    switchMode('image');
});

tabSvg.addEventListener('click', () => {
    initAudio();
    switchMode('svg');
});

// --- Generate Image Expressions Buttons ---
assignUniqueLabels(imageExpressions);
imageExpressions.forEach(expr => {
    const btn = document.createElement('button');
    btn.textContent = expr._btn || expr.label;
    if (expr.id === currentImageExpressionId) btn.classList.add('active');
    
    btn.addEventListener('click', () => {
        initAudio();
        currentImageExpressionId = expr.id;
        
        // Update active button state
        document.querySelectorAll('#image-controls button').forEach(b => b.classList.remove('active'));
        btn.classList.add('active');
        
        // Start animation sequence
        startImageAnimation(expr.id);
        
        // Play robotic sound corresponding to mapped expression
        playRoboticSound(expr.sound);
    });
    
    imageControlsContainer.appendChild(btn);
});

// --- Generate SVG Expressions Buttons ---
assignUniqueLabels(svgFaces);
svgFaces.forEach(expr => {
    const btn = document.createElement('button');
    btn.textContent = expr._btn || expr.label;
    if (expr.id === currentSvgExpressionId) btn.classList.add('active');
    
    btn.addEventListener('click', () => {
        initAudio();
        currentSvgExpressionId = expr.id;
        
        // Update active button state
        document.querySelectorAll('#svg-controls button').forEach(b => b.classList.remove('active'));
        btn.classList.add('active');
        
        // Load the static SVG file
        startSvgFace(expr.id);
        
        // Play robotic sound corresponding to mapped expression
        playRoboticSound(expr.sound);
    });
    
    svgControlsContainer.appendChild(btn);
});

// --- Image Customization Setup ---
// FPS Slider
sliderFps.addEventListener('input', (e) => {
    animationFps = parseInt(e.target.value);
    fpsValue.textContent = animationFps;
    if (activeMode === 'image') {
        // Restart loop with new speed
        startImageAnimation(currentImageExpressionId);
    }
});

// Scale Slider
sliderScale.addEventListener('input', (e) => {
    const val = parseInt(e.target.value);
    scaleValue.textContent = val;
    imageFaceFrame.style.transform = `scale(${val / 100})`;
});

// Hue-Rotate Slider
sliderHue.addEventListener('input', (e) => {
    const val = parseInt(e.target.value);
    imageFaceFrame.style.filter = `hue-rotate(${val}deg) saturate(1.3) brightness(1.2)`;
    // Deselect color dots as it is a custom hue
    document.querySelectorAll('#image-color-controls .color-dot').forEach(d => d.classList.remove('active'));
});

// Generate Image Color Palette Controls
eyeColors.forEach((color, idx) => {
    const dot = document.createElement('div');
    dot.className = 'color-dot';
    dot.style.backgroundColor = color.hex;
    dot.title = color.name;
    dot.setAttribute('data-color', color.hex);
    
    // Default active color is Ciano (matches index 1)
    if (idx === 1) dot.classList.add('active');
    
    dot.addEventListener('click', () => {
        document.querySelectorAll('#image-color-controls .color-dot').forEach(d => d.classList.remove('active'));
        dot.classList.add('active');
        
        applyImageFilter(color.hex);
        
        // Sync Slider Hue value
        if (color.hex.toLowerCase() === '#ffffff') {
            sliderHue.value = 0;
        } else {
            const hsl = hexToHSL(color.hex);
            let hueRotate = hsl.h - 180;
            if (hueRotate < 0) hueRotate += 360;
            sliderHue.value = hueRotate;
        }
        initAudio();
    });
    
    imageColorContainer.appendChild(dot);
});

// --- SVG Customization Setup ---
// Scale Slider
sliderSvgScale.addEventListener('input', (e) => {
    const val = parseInt(e.target.value);
    svgScaleValue.textContent = val;
    svgFaceImage.style.transform = `scale(${val / 100})`;
});

// Hue-Rotate Slider
sliderSvgHue.addEventListener('input', (e) => {
    const val = parseInt(e.target.value);
    svgFaceImage.style.filter = `hue-rotate(${val}deg)`;
    // Deselect color dots as it is a custom hue
    document.querySelectorAll('#svg-color-controls .color-dot').forEach(d => d.classList.remove('active'));
});

// Generate SVG Color Palette Controls
eyeColors.forEach((color, idx) => {
    const dot = document.createElement('div');
    dot.className = 'color-dot';
    dot.style.backgroundColor = color.hex;
    dot.title = color.name;
    dot.setAttribute('data-color', color.hex);
    
    // Default active color is Ciano (matches index 1)
    if (idx === 1) dot.classList.add('active');
    
    dot.addEventListener('click', () => {
        document.querySelectorAll('#svg-color-controls .color-dot').forEach(d => d.classList.remove('active'));
        dot.classList.add('active');
        
        applySvgFilter(color.hex);
        
        // Sync Slider Hue value
        if (color.hex.toLowerCase() === '#ffffff') {
            sliderSvgHue.value = 0;
        } else {
            const hsl = hexToHSL(color.hex);
            let hueRotate = hsl.h - 180;
            if (hueRotate < 0) hueRotate += 360;
            sliderSvgHue.value = hueRotate;
        }
        initAudio();
    });
    
    svgColorContainer.appendChild(dot);
});

// Initialize default scales
imageFaceFrame.style.transform = `scale(${sliderScale.value / 100})`;
svgFaceImage.style.transform = `scale(${sliderSvgScale.value / 100})`;


/* Centralizacao vertical do conjunto olhos+boca.
   Adiciona a classe "has-mouth" ao .face-screen quando a boca esta visivel,
   permitindo que o CSS suba o rosto e compense a boca. Faces so de olhos
   permanecem centradas. */
(function () {
    var mouthEl = document.querySelector('.mouth');
    function updateHasMouth() {
        if (!mouthEl) return;
        var cs = getComputedStyle(mouthEl);
        var vis = cs.display !== 'none' && parseFloat(cs.width) > 0 && parseFloat(cs.height) > 0;
        if (faceScreen.classList.contains('has-mouth') !== vis) {
            faceScreen.classList.toggle('has-mouth', vis);
        }
    }
    new MutationObserver(updateHasMouth).observe(faceScreen, { attributes: true, attributeFilter: ['class'] });
    updateHasMouth();
})();


/* Escala proporcional a tela: faz o rosto caber em qualquer tamanho de tela/janela
   sem cortar, mantendo a proporcao. No tamanho de referencia (ou maior) usa escala 1,
   entao o visual atual nao muda; em telas menores encolhe proporcionalmente. */
(function () {
    var REF_W = 360, REF_H = 280;
    function fitFace() {
        if (!faceScreen) return;
        var w = faceScreen.clientWidth, h = faceScreen.clientHeight;
        if (!w || !h) return;
        var s = Math.min(w / REF_W, h / REF_H);
        if (s > 1) s = 1;
        faceScreen.style.setProperty('--face-scale', s.toFixed(3));
    }
    if (window.ResizeObserver) { new ResizeObserver(fitFace).observe(faceScreen); }
    window.addEventListener('resize', fitFace);
    fitFace();
})();
